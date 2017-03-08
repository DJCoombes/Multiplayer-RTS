#pragma once

/**
* @Author	DJ Coombes
* @date		07th March 2017
* @brief	The external Lua API functions are stored here.
*/

#include "logger.h"
#include "entity.h"
#include "components.h"

class EntityManager;

namespace LuaFuncs {
	extern EntityManager* manager;

	/*!
	  \brief Add the C++ functions to the Lua state.
	*/
	void AddFunctions();

	/*!
	  \brief Log a message from Lua to C++.
	  \param message The message to log.
	*/
	void Log(const std::string& message);

	/*!
	  \brief Create a new entity.
	  \param type Type of entity to create.
	  \param id ID of the player assigned to this entity.
	  \return int containing the id of the newly created entity.
	*/
	int Create(const std::string& type, int id);

	/*!
	  \brief Get en entity from its ID.
	  \param id ID of the entity to get.
	  \return Pointer to the entity.
	*/
	std::shared_ptr<Entity> GetEntity(int id);

	/*!
	  \brief Apply damage to an entity.
	  \param id ID of the entity to damage.
	  \param secondID ID of the second entity that's giving the damage.
	*/
	void Damage(int id, int secondID);

	/*!
	  \brief Destroy an entity.
	  \param id ID of the entity to destroy.
	*/
	void Destroy(int id);

	/*!
	  \brief Set the position of an entity.
	  \param id ID of the entity to set the position of.
	  \param x X position.
	  \param y Y position.
	*/
	void SetPosition(int id, float x, float y);
	
	/*!
	  \brief Change the state of an entity.
	  \param id ID of the entity to change the state of.
	  \param state The name of the state to switch to.
	*/
	void ChangeState(int id, std::string& state);

	/*!
	  \brief Check if an entity has a state component.
	  \param id ID of the entity to check.
	  \return bool that's true if the entity has a state component, false otherwise.
	*/
	bool HasStateComponent(int id);

	/*!
	  \brief Get the type of state that an entity is using.
	  \param id ID of the entity.
	  \return string containing the type of state the entity uses.
	*/
	std::string GetStateType(int id);

	/*!
	  \brief Get the current state that an entity is in.
	  \param id ID of the entity.
	  \return string containing the state the entity is in.
	*/
	std::string GetState(int id);

	/*!
	  \brief Set the state of an entity.
	  \param id ID of the entity.
	  \param state Name of the state to switch to.
	*/
	void SetState(int id, std::string state);

	/*!
	  \brief Set an entity to run through the start of a state.
	  \param id ID of the entity.
	  \param starting True if the entity should enter the start of a state, false otherwise.
	*/
	void SetStartState(int id, bool starting);

	/*!
	  \brief Check if an entity is starting a new state.
	  \param id ID of the entity.
	  \return True if the entity is starting a new state, false otherwise.
	*/
	bool GetStarting(int id);
}