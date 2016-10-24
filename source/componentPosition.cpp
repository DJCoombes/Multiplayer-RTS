#include "componentPosition.h"

#include <iostream>

#include <LuaBridge.h>

ComponentPosition::ComponentPosition(luabridge::LuaRef& componentTable) {
	luabridge::LuaRef pos = componentTable["position"];
	auto x = pos[1].cast<float>();
	auto y = pos[2].cast<float>();

	position.x = x;
	position.y = y;
	position.z = 0;
}

std::shared_ptr<ComponentBase> ComponentPosition::Clone() const {
	return std::make_shared<ComponentPosition>(*this);
}
