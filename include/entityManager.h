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
using EntityTemplate = std::unordered_map<std::string, std::shared_ptr<Entity>>;
using ComponentFactory = std::unordered_map<Components, std::function<std::shared_ptr<ComponentBase>(luabridge::LuaRef&)>>;

class EntityManager {
public:
	/*!
	  \brief Constructor.
	*/
	EntityManager();

	/*!
	  \brief Destructor.
	*/
	~EntityManager();

	/*!
	  \brief Return a shared pointer to an entity.
	  \param id Unique ID of the entity.
	  \return Shared pointer to the entity.
	*/
	std::shared_ptr<Entity> GetEntity(int id);

	/*!
	  \brief Create a new entity from the entities store in the entity template.
	  \param type String type of the entity to create.
	  \return Int with the unique ID of the newly created entity.
	*/
	int Create(const std::string& type);

	/*!
	\brief Create a template of an entity and store it.
	\param type Type of the entity to create.
	*/
	int CreateEntity(const std::string& type);

	/*!
	  \brief Clears all the stored entities.
	*/
	void Clear();

	/*!
	  \brief Add the currently queued entities to the entity vector.
	*/
	void AddQueuedEntities();

	/*!
	  \brief Destroy all the currently queued entities.
	*/
	void DestroyQueuedEntities();

	/*!
	  \brief Lambda function, registers a type of component that can be created from an Enum.
	  \param type Enum of the type of component to be registered.
	  \return Shared pointer to a new component.
	*/
	template<typename T>
	void RegisterComponent(const Components& type) {
		m_componentFactory[type] = [this](luabridge::LuaRef& temp) -> std::shared_ptr<T> {
			return std::make_shared<T>(temp);
		};
	}

private:
	unsigned int		m_idCounter; //!< Current unique ID available.
	EntityContainer		m_entities; //!< Vector of shared pointers to entities.
	EntityContainer		m_entityQueue; //!< Vector of entities to be added.
	EntityContainer		m_destroyQueue; //!< Vector of entities to be destroyed.
	EntityTemplate		m_entityTemplates; //!< Map of template entities.
	ComponentFactory	m_componentFactory; //!< Component factory for automating component creation.
	lua_State*			m_lua; //!< Pointer to the current Lua state.
	std::unordered_map<std::string, Components> m_componentMap; //!< Map of string to component enums, used for automating creation.

};