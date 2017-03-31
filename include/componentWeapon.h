#pragma once

/**
* @Author	DJ Coombes
* @date		15th March 2017
* @brief	Component used for storing weapon data.
*/

#include "componentBase.h"

#include <SFML/System/Clock.hpp>

struct ComponentWeapon : public ComponentBase {
	/*!
	  \brief Common constructor for all components.
	  \param componentTable Table containing the component data.
	*/
	ComponentWeapon(luabridge::LuaRef& componentTable);

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

	std::string m_weaponType; //!< Type of weapon the entity uses.	
	float m_originX; //!< Start X position.
	float m_originY; //!< Start Y position.
	int m_targetID; //!< ID of the current target.
	sf::Time m_rateOfFire; //!< Time in which the weapon can shoot.
	sf::Clock m_lastShot; //!< Time since the last shot.
	bool m_canFire; //!< Can the weapon currently fire.
};