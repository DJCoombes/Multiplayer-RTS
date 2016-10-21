#pragma once

#include "componentBase.h"

#include <iostream>
#include <string>

class ComponentCollision : public ComponentBase {
public:
	ComponentCollision() {}
	ComponentCollision(luabridge::LuaRef& componentTable);

	std::string test;
};