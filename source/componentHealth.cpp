/**
* @Author	DJ Coombes
* @date		16th March 2017
* @brief	Component used for storing health data.
*/

#include "componentHealth.h"

#include <LuaBridge.h>

ComponentHealth::ComponentHealth(luabridge::LuaRef& componentTable) {
	luabridge::LuaRef health = componentTable["health"];
	m_health = health.cast<float>();
}

std::shared_ptr<ComponentBase> ComponentHealth::Clone() const {
	return std::make_shared<ComponentHealth>(*this);
}

sf::Packet& ComponentHealth::Get(sf::Packet& packet) const {
	return packet << m_enabled << m_health;
}

sf::Packet& ComponentHealth::Set(sf::Packet& packet) {
	return packet >> m_enabled >> m_health;
}