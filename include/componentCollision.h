#pragma once

/**
* @Author	DJ Coombes
* @date		21 October 2016
* @brief	Component used for storing collision data.
*/

#include "componentBase.h"

#include <iostream>
#include <string>

#include <SFML/Graphics/Rect.hpp>

struct ComponentCollision : public ComponentBase {
	/*!
	  \brief Common constructor for all components.
	  \param componentTable Table containing the component data.
	*/
	ComponentCollision(luabridge::LuaRef& componentTable);

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

	sf::FloatRect m_bounds; //!< Collision bounds of the entity.
	bool m_colliding; //!< True if the entity is colliding.
};