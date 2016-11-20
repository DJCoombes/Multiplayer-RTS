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

class ComponentCollision : public ComponentBase {
public:
	ComponentCollision() {}
	ComponentCollision(luabridge::LuaRef& componentTable);

	virtual std::shared_ptr<ComponentBase> Clone() const;

	sf::Packet& Get(sf::Packet& packet) const;
	sf::Packet& Set(sf::Packet& packet);

	sf::FloatRect m_bounds;
};