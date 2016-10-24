#pragma once

#include "componentBase.h"

#include <SFML\System\Vector3.hpp>

class ComponentPosition : public ComponentBase {
public:
	ComponentPosition(luabridge::LuaRef& componentTable);

	virtual std::shared_ptr<ComponentBase> Clone() const;

	sf::Vector3f position;
};