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