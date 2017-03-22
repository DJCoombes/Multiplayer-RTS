/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	The entity manager will create, delete and manage all entities.
*/

#include "entityManager.h"

#include <algorithm>
#include <experimental/filesystem>
#include <iterator>

#include <LuaBridge.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "logger.h"
#include "components.h"
#include "entity.h"

namespace fs = std::experimental::filesystem;

EntityManager::EntityManager() {
	// Set the starting ID.
	m_idCounter = 1;
	// Create a new Lua state and open the Lua libraries.
	m_lua = luabridge::luaL_newstate();
	luabridge::luaL_openlibs(m_lua);

	// initialize the components and component maps.
	m_componentMap["ComponentCollision"] = Components::COLLISION;
	RegisterComponent<ComponentCollision>(Components::COLLISION);

	m_componentMap["ComponentGraphics"] = Components::GRAPHICS;
	RegisterComponent<ComponentGraphics>(Components::GRAPHICS);

	m_componentMap["ComponentPosition"] = Components::POSITION;
	RegisterComponent<ComponentPosition>(Components::POSITION);

	m_componentMap["ComponentMovement"] = Components::MOVEMENT;
	RegisterComponent<ComponentMovement>(Components::MOVEMENT);

	m_componentMap["ComponentSelect"] = Components::SELECT;
	RegisterComponent<ComponentSelect>(Components::SELECT);

	m_componentMap["ComponentState"] = Components::STATE;
	RegisterComponent<ComponentState>(Components::STATE);

	m_componentMap["ComponentWeapon"] = Components::WEAPON;
	RegisterComponent<ComponentWeapon>(Components::WEAPON);

	m_componentMap["ComponentHealth"] = Components::HEALTH;
	RegisterComponent<ComponentHealth>(Components::HEALTH);

	m_componentMap["ComponentDamage"] = Components::DAMAGE;
	RegisterComponent<ComponentDamage>(Components::DAMAGE);

	for (auto& i : fs::directory_iterator("resources/entities")) {
		std::string name = i.path().string();
		name.erase(0, 19);
		name.erase(name.length() - 4, 4);
		try {
			luahelp::LoadScript(m_lua, i.path().string());
			luahelp::GetLuaKeys(m_lua);
			CreateEntity(name);
		}
		catch (luabridge::LuaException const& e) {
			LOG(ERRORR) << "Failed to create entity: " << name;
			LOG(ERRORR) << e.what();
		}
	}

	m_entities.reserve(100000);
}

EntityManager::~EntityManager() {}

std::shared_ptr<Entity> EntityManager::GetEntity(int id) {
	// Check if the entity is in the current entity vector and return it.
	for (auto& i : m_entities) {
		if (i->GetID() == id)
			return i;
	}

	if (m_entityQueue.empty())
		return nullptr;

	// Check if the entity is in the entity queue and return in.
	for (auto& i : m_entityQueue) {
		if (i->GetID() == id)
			return i;
	}

	return nullptr;
}

EntityContainer& EntityManager::GetEntities() {
	return m_entities;
}
#ifdef SERVER
int EntityManager::Create(const std::string& type) {
#else
int EntityManager::Create(const std::string& type, int id) {
#endif
	// Find the entity that's required in the template map.
	auto entity = m_entityTemplates.find(type);
	if (entity == m_entityTemplates.end()) {
		LOG(WARNING) << "Entity not found!";
		return -1;
	}
	// Create a copy of the entity that's in the template map, give it an ID and push it on to the queue.
	std::shared_ptr<Entity> newEntity = std::make_shared<Entity>(*entity->second);
#ifdef SERVER
	newEntity->SetID(++m_idCounter);
#else
	newEntity->SetID(id);
#endif
	m_entityQueue.push_back(newEntity);
#ifdef SERVER
	// If the server's creating the entity then all the clients need to know about it as well so broad cast an entity creation packet.
	sf::Packet packet;
	SetPacketType(PacketType::ENTITYCREATION, packet);
	packet << type << newEntity->GetID();
	LOG(DEBUG) << type << " created.";
	m_server->Broadcast(packet);
#endif
	return newEntity->GetID();
}

void EntityManager::Destroy(int id) {
	auto temp = GetEntity(id);
	if (temp == nullptr)
		return;

	for (auto& i : m_destroyQueue)
		if (i->GetID() == id)
			return;

	m_destroyQueue.push_back(temp);
#ifdef SERVER
	sf::Packet packet;
	SetPacketType(PacketType::ENTITYDESTRUCTION, packet);
	packet << id;
	m_server->Broadcast(packet);
#endif
}

void EntityManager::Clear() {
	m_entities.clear();
	m_entityQueue.clear();
	m_destroyQueue.clear();
	m_idCounter = 1;
}

void EntityManager::CreateEntity(const std::string& type) {
	// Find the table keys relating to the entity, these will be used to know what components to create.
	auto keys = luahelp::GetTableKeys(m_lua, type);
	// Get the entity table relating to this entity.
	luabridge::LuaRef entityTable = luabridge::getGlobal(m_lua, type.c_str());
	// Get the type of entity that's going to be created.
	auto entityType = entityTable["Type"];
	// Check to see if this entity already has a template.
	auto temp = m_entityTemplates.find(type);
	if (temp != m_entityTemplates.end()) {
		LOG(INFO) << "Entity (" << type << ") already exists.";
		return;
	}
	// Create a shared pointer to a new entity.
	auto entity = std::make_shared<Entity>();
	// Set the name and type.
	entity->SetName(type);
	entity->SetType(entityType);
	entity->SetLuaState(m_lua);
	// For each of the components in the keys, create a shared component of that component type.
	for (auto& componentName : keys) {
		if (componentName == "Type")
			continue;
		// Create a new component using the component factory.
		Components componentEnum = m_componentMap.at(componentName);
		auto newComponent = m_componentFactory.find(componentEnum);
		if (newComponent == m_componentFactory.end())
			continue;
		// Find the component table inside the entity table.
		luabridge::LuaRef compTable = entityTable[componentName];
		auto component = newComponent->second(compTable);
		// Add the component using it's type.
		entity->AddComponent(std::type_index(typeid(*component)), component);
	}
	m_entityTemplates[type] = entity;
}

void EntityManager::AddQueuedEntities() {
	std::move(std::begin(m_entityQueue), std::end(m_entityQueue), std::back_inserter(m_entities));
	m_entityQueue.clear();
}

void EntityManager::DestroyQueuedEntities() {
	for (auto& i : m_destroyQueue) {
		auto it = std::find(m_entities.begin(), m_entities.end(), i);
		if (it != m_entities.end()) {
			it = m_entities.erase(it);
		}
		else {
			auto it = std::find(m_entityQueue.begin(), m_entityQueue.end(), i);
				if (it != m_entityQueue.end())
					it = m_entityQueue.erase(it);
				else
					LOG(DEBUG) << "Cannot destroy " << i->GetID() << ", entity not found.";
		}
	}

	m_destroyQueue.clear();
}

lua_State* EntityManager::GetLuaState() {
	return m_lua;
}

#ifdef SERVER
void EntityManager::AddServerInstance(Server* server) {
	m_server = server;
}
#endif