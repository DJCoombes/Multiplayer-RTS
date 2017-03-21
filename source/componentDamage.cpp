/**
* @Author	DJ Coombes
* @date		17th March 2017
* @brief	Component used for storing damage data.
*/

#include "componentDamage.h"

#include <LuaBridge.h>

ComponentDamage::ComponentDamage(luabridge::LuaRef& componentTable) {
	m_damage = componentTable["damage"].cast<float>();
	m_range = componentTable["range"].cast<int>();
}

std::shared_ptr<ComponentBase> ComponentDamage::Clone() const {
	return std::make_shared<ComponentDamage>(*this);
}

sf::Packet& ComponentDamage::Get(sf::Packet& packet) const {
	return packet << m_enabled << m_damage << m_range
		<< m_startPos.x << m_startPos.y;
}

sf::Packet& ComponentDamage::Set(sf::Packet& packet) {
	return packet >> m_enabled >> m_damage >> m_range
		>> m_startPos.x >> m_startPos.y;
}