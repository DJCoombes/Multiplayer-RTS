/**
* @Author	DJ Coombes
* @date		21 October 2016
* @brief	Component used for storing collision data.
*/

#include "componentCollision.h"

#include <iostream>
#include <LuaBridge.h>

ComponentCollision::ComponentCollision(luabridge::LuaRef& componentTable) : 
	m_colliding(false) {
	auto width = componentTable["width"];
	auto height = componentTable["height"];

	m_bounds = sf::FloatRect(0, 0, width, height);
}

std::shared_ptr<ComponentBase> ComponentCollision::Clone() const {
	return std::make_shared<ComponentCollision>(*this);
}

sf::Packet& ComponentCollision::Get(sf::Packet& packet) const {
	return packet << m_enabled << m_bounds.left << m_bounds.top 
		<< m_bounds.width << m_bounds.height << m_colliding;
}

sf::Packet& ComponentCollision::Set(sf::Packet& packet) {
	return packet >> m_enabled >> m_bounds.left >> m_bounds.top
		>> m_bounds.width >> m_bounds.height >> m_colliding;
}