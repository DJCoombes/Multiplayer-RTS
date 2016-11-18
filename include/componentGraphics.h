#pragma once

/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Used to store graphics data in a component.
*/

#include <string>

#include <componentBase.h>

class ComponentGraphics : public ComponentBase {
public:
	ComponentGraphics(luabridge::LuaRef& componentTable);

	virtual std::shared_ptr<ComponentBase> Clone() const;

	sf::Packet& Get(sf::Packet& packet) const;
	sf::Packet& Set(sf::Packet& packet);

	std::string name;
};