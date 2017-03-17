/**
* @Author	DJ Coombes
* @date		27 February 2017
* @brief	Component used for storing state data.
*/

#include "componentState.h"

#include <iostream>

#include <LuaBridge.h>

ComponentState::ComponentState(luabridge::LuaRef& componentTable) : m_start(true),
	m_switching(false) {
	auto state = componentTable["currentState"];
	auto type = componentTable["stateMachineType"];

	m_state = state.cast<std::string>();
	m_stateMachineType = type.cast<std::string>();
}

std::shared_ptr<ComponentBase> ComponentState::Clone() const {
	return std::make_shared<ComponentState>(*this);
}

sf::Packet& ComponentState::Get(sf::Packet& packet) const {
	return packet << m_enabled << m_state << m_stateMachineType << m_start << m_switching;
}

sf::Packet& ComponentState::Set(sf::Packet& packet) {
	return packet >> m_enabled << m_state >> m_stateMachineType >> m_start >> m_switching;
}