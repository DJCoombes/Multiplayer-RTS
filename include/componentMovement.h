#pragma once

/**
* @Author	DJ Coombes
* @date		19 November 2016
* @brief	Component used for storing movement data.
*/

#include "componentBase.h"

#include <iostream>

#include <SFML/System/Vector2.hpp>

struct ComponentMovement : public ComponentBase {
	/*!
	  \brief Common constructor for all components.
	  \param componentTable Table containing the component data.
	*/
	ComponentMovement(luabridge::LuaRef& componentTable);

	/*!
	  \brief Set the move to position of the entity.
	  \param pos The new position to move to.
	*/
	void MoveTo(sf::Vector2f pos);

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

	int m_speed; //!< Speed of the entity.
	sf::Vector2f m_moveTo; //!< Position the entity is moving to.
	sf::Vector2f m_velocity; //!< Velocity needed to reach the position at correct speed.
	bool m_atPos; //!< True if the entity is at the move to position.
};