#pragma once

/**
* @Author	DJ Coombes
* @date		21 October 2016
* @brief	Component used for storing collision data.
*/

#include "componentBase.h"

#include <iostream>
#include <string>

class ComponentCollision : public ComponentBase {
public:
	ComponentCollision() {}
	ComponentCollision(luabridge::LuaRef& componentTable);

	virtual std::shared_ptr<ComponentBase> Clone() const;

	std::string test;
};