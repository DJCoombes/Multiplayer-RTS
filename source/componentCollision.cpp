#include "componentCollision.h"

#include <iostream>
#include <LuaBridge.h>

ComponentCollision::ComponentCollision(luabridge::LuaRef& componentTable) {
	auto string = componentTable["test"];

	test = string.cast<std::string>();
}