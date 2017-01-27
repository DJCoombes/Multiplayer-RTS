/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	The entity manager will create, delete and manage all entities.
*/

#include "entityManager.h"

#include <algorithm>

#include <LuaBridge.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "luaHelperFunctions.h"
#include "logger.h"
#include "componentCollision.h"
#include "componentGraphics.h"
#include "componentPosition.h"
#include "componentMovement.h"
#include "componentSelect.h"

EntityManager::EntityManager() {
	// Set the starting ID.
	m_idCounter = 1;
	// Create a new lua state and open the Lua libraries.
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

	// Load the Lua scripts into the state and put the keys on the Lua stack.
	luahelp::LoadScript(m_lua, "./resources/entities/test.lua");
	luahelp::GetLuaKeys(m_lua);

	// Create a test entity template.
	CreateEntity("test");
}

EntityManager::~EntityManager() {}

std::shared_ptr<Entity> EntityManager::GetEntity(int id) {
	// Check if the entity is in the current entity vector and return it.
	for (auto& i : m_entities) {
		if (i->GetID() == id)
			return i;
	}
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

int EntityManager::Create(const std::string& type) {
	// Find the entity that's required in the template map.
	auto entity = m_entityTemplates.find(type);
	if (entity == m_entityTemplates.end()) {
		LOG(WARNING) << "Entity not found!";
		return -1;
	}
	// Create a copy of the entity that's in the template map, give it an ID and push it on to the queue.
	std::shared_ptr<Entity> newEntity = std::make_shared<Entity>(*entity->second);
	newEntity->SetID(++m_idCounter);
	m_entityQueue.push_back(newEntity);
#ifdef SERVER
	// If the server's creating the entity then all the clients need to know about it as well so broad cast an entity creation packet.
	sf::Packet packet;
	SetPacketType(PacketType::ENTITYCREATION, packet);
	packet << type;
	LOG(INFO) << type << " created.";
	m_server->Broadcast(packet);
#endif
	return newEntity->GetID();
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
	m_entities.insert(m_entities.end(), m_entityQueue.begin(), m_entityQueue.end());
	m_entityQueue.clear();
}

void EntityManager::DestroyQueuedEntities() {
	for (auto& i : m_destroyQueue) {
		m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), i), m_entities.end());
	}
	m_destroyQueue.clear();
}

#ifdef SERVER
void EntityManager::AddServerInstance(Server* server) {
	m_server = server;
}
#endif