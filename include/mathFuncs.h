#pragma once

/**
* @Author	DJ Coombes
* @date		19th November 2016
* @brief	Common math functions.
*/

#include <math.h>
#include <iostream>

#include <SFML/System/Vector2.hpp>

inline sf::Vector2f calcVelocity(sf::Vector2f currentPos, sf::Vector2f targetPos, int speed, float timeStep) {
	sf::Vector2f vel(targetPos.x - currentPos.x, targetPos.y - currentPos.y);
	float magnitude = std::sqrt(vel.x * vel.x + vel.y * vel.y);
	
	return sf::Vector2f(vel.x / magnitude * (speed * timeStep), vel.y / magnitude * (speed * timeStep));
}