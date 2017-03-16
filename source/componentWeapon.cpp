/**
* @Author	DJ Coombes
* @date		15th March 2017
* @brief	Component used for storing weapon data.
*/

#include "componentWeapon.h"

#include <LuaBridge.h>

ComponentWeapon::ComponentWeapon(luabridge::LuaRef& componentTable) {
	auto type = componentTable["type"];
	auto fireRate = componentTable["rateOfFire"];

	m_originX = 0;
	m_originY = 0;

	m_targetID = NULL;

	m_rateOfFire = sf::seconds(fireRate);
	m_canFire = true;
}

std::shared_ptr<ComponentBase> ComponentWeapon::Clone() const {
	return std::make_shared<ComponentWeapon>(*this);
}

sf::Packet& ComponentWeapon::Get(sf::Packet& packet) const {
	return packet << m_weaponType << m_originX << m_originY << m_targetID
		<< m_rateOfFire.asSeconds()	<< m_canFire;
}

sf::Packet& ComponentWeapon::Set(sf::Packet& packet) {
	float rateOfFire;
	return packet >> m_weaponType >> m_originX >> m_originY >> m_targetID
		>> rateOfFire >> m_canFire;
	m_rateOfFire = sf::seconds(rateOfFire);
}