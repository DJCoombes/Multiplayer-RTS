#pragma once

/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	The entity class is a container for components. Also stores generic information.
*/

#include <string>
#include <map>
#include <typeindex>
#include <typeinfo>
#include <memory>

#include "componentBase.h"
#include "luaHelperFunctions.h"

using EntityID = unsigned int;

class Entity {
public:
	/*!
	  \brief Constructor.
	*/
	Entity() {}

	/*!
	  \brief Deep copy constructor.
	*/
	Entity(const Entity& entity) {
		m_name = entity.m_name;
		m_type = entity.m_type;

		for (auto& i : entity.m_components) {
			m_components[i.first] = i.second->Clone();
		}
	}

	/*!
	  \brief Destructor.
	*/
	~Entity() {}

	/*!
	  \brief Add a component to this entity.
	  \param type Type index of the component to add.
	  \param component Shared pointer to the component.
	*/
	void AddComponent(std::type_index type, std::shared_ptr<ComponentBase> component) {
		m_components[type] = component;
	}

	/*!
	  \brief Remove a component from this entity.
	  \param type Type of component to remove.
	*/
	void RemoveComponent(std::type_index type) {
		m_components.erase(type);
	}

	/*!
	  \brief Set the type of the entity.
	  \param type String of the type.
	*/
	void SetType(const std::string& type) {
		m_type = type;
	}

	/*!
	  \brief Set the name of the entity.
	  \param name String of the name.
	*/
	void SetName(const std::string& name) {
		m_name = name;
	}

	/*!
	  \brief Set the unique ID of the entity.
	  \param ID ID of the entity.
	*/
	void SetID(EntityID id) {
		m_id = id;
	}

	/*!
	  \brief Get the type of the entity.
	  \return String containing the type.
	*/
	std::string GetType() {
		return m_type;
	}

	/*!
	  \brief Get the entities unique ID.
	  \return Unsigned integer containing the entities unique ID.
	*/
	EntityID GetID() {
		return m_id;
	}

	/*!
	  \brief Get a shared pointer to a component.
	  \return Shared pointer if entity has component, otherwise nullptr.
	*/
	template <typename T>
	std::shared_ptr<T> Get() {
		auto i = m_components.find(std::type_index(typeid(T)));
		if (i != m_components.end()) {
			if (i->second->IsEnabled()) {
				return std::dynamic_pointer_cast<T>(i->second);
			}
		}
		return nullptr;
	}
private:
	std::string		m_name; //!< Name of the entity.
	std::string		m_type; //!< Type of the entity.
	EntityID		m_id; //!< Unique ID of the entity.

	std::map<std::type_index, std::shared_ptr<ComponentBase>> m_components; //!< Map of types to shared pointer to components.
};