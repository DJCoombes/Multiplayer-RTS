/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Used to store information on whether the component is selected / selectable.
*/

#include "componentSelect.h"

#include <iostream>

#include <LuaBridge.h>

ComponentSelect::ComponentSelect(luabridge::LuaRef& componentTable) {
	auto selectableTemp = componentTable["selectable"];
	selectable = selectableTemp;
	selected = false;
}

std::shared_ptr<ComponentBase> ComponentSelect::Clone() const {
	return std::make_shared<ComponentSelect>(*this);
}

sf::Packet& ComponentSelect::Get(sf::Packet& packet) const {
	return packet << m_enabled << selectable << selected;
}

sf::Packet& ComponentSelect::Set(sf::Packet& packet) {
	return packet >> m_enabled >> selectable >> selected;
}