/**
* @Author	DJ Coombes
* @date		21 October 2016
* @brief	Component used for storing collision data.
*/

#include "componentCollision.h"

#include <iostream>
#include <LuaBridge.h>

ComponentCollision::ComponentCollision(luabridge::LuaRef& componentTable) {
	auto string = componentTable["test"];

	test = string.cast<std::string>();
}

std::shared_ptr<ComponentBase> ComponentCollision::Clone() const {
	return std::make_shared<ComponentCollision>(*this);
}

sf::Packet& ComponentCollision::Get(sf::Packet& packet) const {
	return packet << m_enabled << test;
}

sf::Packet& ComponentCollision::Set(sf::Packet& packet) {
	return packet >> m_enabled >> test;
}