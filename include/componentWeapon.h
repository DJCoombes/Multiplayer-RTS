#pragma once

/**
* @Author	DJ Coombes
* @date		15th March 2017
* @brief	Component used for storing weapon data.
*/

#include "componentBase.h"

#include <SFML/System/Clock.hpp>

struct ComponentWeapon : public ComponentBase {
	ComponentWeapon(luabridge::LuaRef& componentTable);

	virtual std::shared_ptr<ComponentBase> Clone() const;

	sf::Packet& Get(sf::Packet& packet) const;
	sf::Packet& Set(sf::Packet& packet);

	std::string m_weaponType; //!< Type of weapon the entity uses.	
	float m_originX; //!< Start X position.
	float m_originY; //!< Start Y position.
	int m_targetID; //!< ID of the current target.
	sf::Time m_rateOfFire; //!< Time in which the weapon can shoot.
	sf::Clock m_lastShot; //!< Time since the last shot.
	bool m_canFire; //!< Can the weapon currently fire.
};