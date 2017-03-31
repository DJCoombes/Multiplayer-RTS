#pragma once

/**
* @Author	DJ Coombes
* @date		19th November 2016
* @brief	Common math functions.
*/

#include <math.h>
#include <iostream>

#include <SFML/System/Vector2.hpp>

/*!
  \brief Calculate the velocity needed to reach a destination at a given speed.
  \param currentPosition The current position.
  \param targetPos The position that it's going to.
  \param speed The speed it's traveling at.
  \param timeStep The time since the last update.
  \return sf::Vector2f A 2D float vector containing the velocity.
*/
inline sf::Vector2f CalcVelocity(sf::Vector2f currentPos, sf::Vector2f targetPos, int speed, float timeStep) {
	sf::Vector2f vel(targetPos.x - currentPos.x, targetPos.y - currentPos.y);
	float magnitude = std::sqrt(vel.x * vel.x + vel.y * vel.y);
	
	return sf::Vector2f(vel.x / magnitude * (speed * timeStep), vel.y / magnitude * (speed * timeStep));
}