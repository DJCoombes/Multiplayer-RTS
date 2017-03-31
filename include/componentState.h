#pragma once

/**
* @Author	DJ Coombes
* @date		27 February 2017
* @brief	Component used for storing state data.
*/

#include <string>

#include "componentBase.h"

struct ComponentState : public ComponentBase {
	/*!
	  \brief Common constructor for all components.
	  \param componentTable Table containing the component data.
	*/
	ComponentState(luabridge::LuaRef& componentTable);

	/*!
	  \brief Clones this object and returns a shared pointer to the new object, must be implemented in derived objects.
	  \return Shared_ptr to thew new object.
	*/
	virtual std::shared_ptr<ComponentBase> Clone() const;

	/*!
	  \brief Used to stream data from the component into SF packets.
	  \param packet The packet to stream data into.
	  \return The packet with the data in.
	*/
	sf::Packet& Get(sf::Packet& packet) const;

	/*!
	  \brief Used to stream data from SF packets into the component.
	  \param packet The packer to stream data from.
	  \param The updated packet.
	*/
	sf::Packet& Set(sf::Packet& packet);

	std::string m_state; //!< State the entity is currently in.
	std::string m_stateMachineType; //!< Type of state machine.
	bool m_start; //!< True if the entity is entering the current state.
	bool m_switching; //!< True if the entity is switching to a new state.
};