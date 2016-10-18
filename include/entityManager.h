#pragma once

/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	The entity manager will create, delete and manage all entities.
*/

#include <vector>
#include <unordered_map>
#include <functional>

#include "entity.h"

/*!
  \brief Available components.
*/
enum Components {
	COLLISION,
	MOVEMENT
};

using EntityContainer = std::vector<std::shared_ptr<Entity>>;
using ComponentFactory = std::unordered_map<Components, std::function<std::shared_ptr<ComponentBase>()>>;

class EntityManager {
public:
	EntityManager();

	~EntityManager();

	std::shared_ptr<Entity> GetEntity(int id);

	void Clear();

	int CreateEntity(std::string& type);

	void AddQueuedEntities();

	void DestroyQueuedEntities();

	template<typename T>
	void RegisterComponent(const Components& type) {
		m_componentFactory[type] = [this]() -> std::shared_ptr<ComponentBase> {
			return std::make_shared<T>();
		};
	}

private:
	unsigned int		m_idCounter;
	EntityContainer		m_entities;
	EntityContainer		m_entityQueue;
	EntityContainer		m_destroyQueue;
	ComponentFactory	m_componentFactory;
	lua_State*			m_lua;
	std::unordered_map<std::string, Components> m_componentMap;

};