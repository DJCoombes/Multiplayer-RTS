/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Used to store graphics data in a component.
*/

#include "componentGraphics.h"

#include <LuaBridge.h>

#include "logger.h"

ComponentGraphics::ComponentGraphics(luabridge::LuaRef& componentTable) {
	auto nameTemp = componentTable["name"];

	if (nameTemp.isString())
		m_name = nameTemp.cast<std::string>();
	else
		LOG(ERRORR) << "ComponentGraphics.name is not a string!";
}

std::shared_ptr<ComponentBase> ComponentGraphics::Clone() const {
	return std::make_shared<ComponentGraphics>(*this);
}

sf::Packet& ComponentGraphics::Get(sf::Packet& packet) const {
	return packet << m_enabled << m_name;
}

sf::Packet& ComponentGraphics::Set(sf::Packet& packet) {
	return packet >> m_enabled >> m_name;
}