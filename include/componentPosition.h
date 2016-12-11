#pragma once

/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Used to store position data in a component.
*/

#include "componentBase.h"

#include <SFML\System\Vector2.hpp>

struct ComponentPosition : public ComponentBase {
	ComponentPosition(luabridge::LuaRef& componentTable);

	virtual std::shared_ptr<ComponentBase> Clone() const;

	sf::Vector2f m_position; //!< Position of the entity.
	sf::Vector2f m_size; //!< Size of the entity.

	sf::Packet& Get(sf::Packet& packet) const;
	sf::Packet& Set(sf::Packet& packet);
};