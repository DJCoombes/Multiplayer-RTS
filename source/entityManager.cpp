/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	The entity manager will create, delete and manage all entities.
*/

#include "entityManager.h"

#include <algorithm>

#include "luaHelperFunctions.h"
#include "componentCollision.h"

#include <LuaBridge.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

EntityManager::EntityManager() {
	m_idCounter = 1;
	m_lua = luabridge::luaL_newstate();
	luabridge::luaL_openlibs(m_lua);

	m_componentMap["ComponentCollision"] = Components::COLLISION;
	RegisterComponent<ComponentCollision>(Components::COLLISION);

	luahelp::LoadScript(m_lua, "./resources/entities/test.lua");
	luahelp::LoadGetKeysFunction(m_lua);

	CreateEntity("test");

	int test = Create("test");
	auto ent =  GetEntity(test);
	auto comp = ent->Get<ComponentCollision>();
	std::cout << comp->test << std::endl;
}

EntityManager::~EntityManager() {}

std::shared_ptr<Entity> EntityManager::GetEntity(int id) {
	for (auto& i : m_entities) {
		if (i->GetID() == id)
			return i;
	}

	for (auto& i : m_entityQueue) {
		if (i->GetID() == id)
			return i;
	}

	return nullptr;
}

int EntityManager::Create(const std::string& type) {
	auto entity = m_entityTemplates.find(type);
	if (entity == m_entityTemplates.end())
		return -1;
	std::shared_ptr<Entity> newEntity = std::make_shared<Entity>(*entity->second);
	newEntity->SetID(++m_idCounter);
	m_entityQueue.push_back(newEntity);
	return newEntity->GetID();
}

void EntityManager::Clear() {
	m_entities.clear();
	m_entityQueue.clear();
	m_destroyQueue.clear();
}

void EntityManager::CreateEntity(const std::string& type) {
	auto keys = luahelp::GetTableKeys(m_lua, type);
	luabridge::LuaRef entityTable = luabridge::getGlobal(m_lua, type.c_str());
	auto entityType = entityTable["Type"];

	auto temp = m_entityTemplates.find(type);
	if (temp != m_entityTemplates.end())
		return;	
	auto entity = std::make_shared<Entity>();
	entity->SetName(type);
	entity->SetType(entityType);
	//entity->SetID(++m_idCounter); // Temp code.
	for (auto& componentName : keys) {
		if (componentName == "Type")
			continue;
		Components componentEnum = m_componentMap.at(componentName);
		auto newComponent = m_componentFactory.find(componentEnum);
		if (newComponent == m_componentFactory.end())
			continue;
		luabridge::LuaRef compTable = entityTable[componentName];
		auto component = newComponent->second(compTable);
		std::cout << std::type_index(typeid(*component)).name() << std::endl;
		entity->AddComponent(std::type_index(typeid(*component)), component);
	}
	//m_entityQueue.push_back(entity);
	//return entity->GetID();
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