/**
* @Author	DJ Coombes
* @date		19 November 2016
* @brief	Component used for storing movement data.
*/

#include "componentMovement.h"

#include <LuaBridge.h>

#include "logger.h"

ComponentMovement::ComponentMovement(luabridge::LuaRef& componentTable)
	: m_atPos(true), m_velocity(sf::Vector2f(0, 0)) {
	auto speedTemp = componentTable["speed"];

	m_speed = speedTemp.cast<int>();
}

void ComponentMovement::MoveTo(sf::Vector2f pos) {
	m_moveTo = pos;
	m_velocity = sf::Vector2f(0, 0);
	m_atPos = false;
}

std::shared_ptr<ComponentBase> ComponentMovement::Clone() const {
	return std::make_shared<ComponentMovement>(*this);
}

sf::Packet& ComponentMovement::Get(sf::Packet& packet) const {
	return packet << m_enabled << m_speed;
}

sf::Packet& ComponentMovement::Set(sf::Packet& packet) {
	return packet >> m_enabled >> m_speed;
}