#pragma once

/**
* @Author	DJ Coombes
* @date		16th March 2017
* @brief	Component used for storing health data.
*/

#include "componentBase.h"

struct ComponentHealth : public ComponentBase {
	/*!
	  \brief Common constructor for all components.
	  \param componentTable Table containing the component data.
	*/
	ComponentHealth(luabridge::LuaRef& componentTable);

	/*!
	  \brief Clones this object and returns a shared pointer to the new object, must be implemented in derived objects.
	  \return Shared_ptr to thew new object.
	*/
	virtual std::shared_ptr<ComponentBase> Clone() const;

	/*!
	  \brief Used to stream data from the component into SF packets.
	  \param packet The packet to stream data into.
	  \return The packet with the data in.
	*/
	sf::Packet& Get(sf::Packet& packet) const;

	/*!
	  \brief Used to stream data from SF packets into the component.
	  \param packet The packer to stream data from.
	  \param The updated packet.
	*/
	sf::Packet& Set(sf::Packet& packet);

	float m_health; //!< Health of the entity.
};