#pragma once

/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Used to store information on whether the component is selected / selectable.
*/

#include "componentBase.h"

#include <SFML/Graphics/Rect.hpp>

struct ComponentSelect : public ComponentBase {
	/*!
	  \brief Common constructor for all components.
	  \param componentTable Table containing the component data.
	*/
	ComponentSelect(luabridge::LuaRef& componentTable);

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

	bool m_selectable; //!< True if the entity is selectable.
	bool m_selected; //!< True if the entity is currently selected.

	sf::FloatRect m_selectBox; //!< The box dimensions that can be clicked on to select it.
};