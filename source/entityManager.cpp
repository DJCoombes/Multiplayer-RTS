/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	The entity manager will create, delete and manage all entities.
*/

#include "entityManager.h"

#include <algorithm>

#include "luaHelperFunctions.h"

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

void EntityManager::Clear() {
	m_entities.clear();
	m_entityQueue.clear();
	m_destroyQueue.clear();
}

int EntityManager::CreateEntity(std::string& type) {
	auto entity = std::make_shared<Entity>();
	entity->SetName(type);
	entity->SetID(++m_idCounter);
	auto keys = luahelp::GetTableKeys(m_lua, type);
	luabridge::LuaRef entityTable = luabridge::getGlobal(m_lua, type.c_str());
	auto entityType = entityTable["Type"];
	entity->SetType(entityType);
	for (auto& componentName : keys) {
		if (componentName == "Type")
			continue;
		Components componentEnum = m_componentMap.at(componentName);
		auto newComponent = m_componentFactory.find(componentEnum);
		if (newComponent == m_componentFactory.end()) {
			continue;
		}
		auto component = newComponent->second();
		entity->AddComponent(std::type_index(typeid(component)), component);
	}
	return entity->GetID();
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