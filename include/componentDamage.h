#pragma once

/**
* @Author	DJ Coombes
* @date		17th March 2017
* @brief	Component used for storing damage data.
*/

#include "componentBase.h"

#include <SFML/System/Vector2.hpp>

struct ComponentDamage : public ComponentBase {
	ComponentDamage(luabridge::LuaRef& componentTable);

	virtual std::shared_ptr<ComponentBase> Clone() const;

	sf::Packet& Get(sf::Packet& packet) const;
	sf::Packet& Set(sf::Packet& packet);

	float m_damage; //!< Amount of damage that it can inflict.
	int m_range; //!< Range of the item.
	sf::Vector2f m_startPos; //!< Starting position of the item.
};