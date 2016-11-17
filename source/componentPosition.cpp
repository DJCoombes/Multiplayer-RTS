/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Used to store position data in a component.
*/

#include "componentPosition.h"

#include <iostream>

#include <LuaBridge.h>

ComponentPosition::ComponentPosition(luabridge::LuaRef& componentTable) {
	luabridge::LuaRef pos = componentTable["position"];
	auto x = pos[1].cast<float>();
	auto y = pos[2].cast<float>();

	m_position.x = x;
	m_position.y = y;

	luabridge::LuaRef size = componentTable["size"];
	auto width = size[1].cast<float>();
	auto height = size[2].cast<float>();	

	m_size.x = width;
	m_size.y = height;
}

std::shared_ptr<ComponentBase> ComponentPosition::Clone() const {
	return std::make_shared<ComponentPosition>(*this);
}
