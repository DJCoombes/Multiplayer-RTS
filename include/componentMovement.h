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
	ComponentMovement() {}
	ComponentMovement(luabridge::LuaRef& componentTable);

	void MoveTo(sf::Vector2f pos);

	virtual std::shared_ptr<ComponentBase> Clone() const;

	sf::Packet& Get(sf::Packet& packet) const;
	sf::Packet& Set(sf::Packet& packet);

	int speed;
	sf::Vector2f moveTo;
	sf::Vector2f velocity;
	bool atPos;
};