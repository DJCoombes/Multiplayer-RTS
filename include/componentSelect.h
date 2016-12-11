#pragma once

/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Used to store information on whether the component is selected / selectable.
*/

#include "componentBase.h"

#include <SFML/Graphics/Rect.hpp>

struct ComponentSelect : public ComponentBase {
	ComponentSelect(luabridge::LuaRef& componentTable);

	virtual std::shared_ptr<ComponentBase> Clone() const;

	sf::Packet& Get(sf::Packet& packet) const;
	sf::Packet& Set(sf::Packet& packet);

	bool selectable;
	bool selected;

	sf::FloatRect selectBox;
};