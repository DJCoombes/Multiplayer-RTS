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
	m_selectable = selectableTemp;

	luabridge::LuaRef selectBoxTemp = componentTable["selectBox"];
	auto width = selectBoxTemp[1].cast<float>();
	auto height = selectBoxTemp[2].cast<float>();

	m_selectBox = sf::FloatRect(0, 0, width, height);

	m_selected = false;
}

std::shared_ptr<ComponentBase> ComponentSelect::Clone() const {
	return std::make_shared<ComponentSelect>(*this);
}

sf::Packet& ComponentSelect::Get(sf::Packet& packet) const {
	return packet;
}

sf::Packet& ComponentSelect::Set(sf::Packet& packet) {
	return packet;
}