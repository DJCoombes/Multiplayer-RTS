#pragma once

#include <string>

#include <componentBase.h>

class ComponentGraphics : public ComponentBase {
public:
	ComponentGraphics(luabridge::LuaRef& componentTable);

	virtual std::shared_ptr<ComponentBase> Clone() const;

	std::string name;
};