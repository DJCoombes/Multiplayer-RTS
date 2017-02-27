#pragma once

/**
* @Author	DJ Coombes
* @date		27 February 2017
* @brief	Component used for storing state data.
*/

#include <string>

#include "componentBase.h"

struct ComponentState : public ComponentBase {
	ComponentState(luabridge::LuaRef& componentTable);

	virtual std::shared_ptr<ComponentBase> Clone() const;

	sf::Packet& Get(sf::Packet& packet) const;
	sf::Packet& Set(sf::Packet& packet);

	std::string m_state; //!< State the entity is currently in.
	std::string m_stateMachineType; //!< Type of state machine.
	bool m_start; //!< True if the entity is entering the current state.
	bool m_switching; //!< True if the entity is switching to a new state.
};