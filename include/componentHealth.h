#pragma once

/**
* @Author	DJ Coombes
* @date		16th March 2017
* @brief	Component used for storing health data.
*/

#include "componentBase.h"

struct ComponentHealth : public ComponentBase {
	ComponentHealth(luabridge::LuaRef& componentTable);

	virtual std::shared_ptr<ComponentBase> Clone() const;

	sf::Packet& Get(sf::Packet& packet) const;
	sf::Packet& Set(sf::Packet& packet);

	float m_health; //!< Health of the entity.
};