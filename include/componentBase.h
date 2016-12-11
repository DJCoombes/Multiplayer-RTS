#pragma once

/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	This base component stores common methods and variables for all components.
*/

#include <memory>

#include <SFML/Network/Packet.hpp>

//!< Forward declaration.
namespace luabridge {
	class LuaRef;
}

struct ComponentBase {
	/*!
	  \brief Common constructor for all components.
	*/
	ComponentBase() : m_enabled(true) {}

	/*!
	  \brief Virtual destructor, needs to be overridden in child classes.
	*/
	virtual ~ComponentBase() {}


	/*!
	  \brief Clones this object and returns a shared pointer to the new object, must be implemented in derived objects.
	  \return Shared_ptr to thew new object.
	*/
	virtual std::shared_ptr<ComponentBase> Clone() const {
		return std::make_shared<ComponentBase>(*this);
	};

	/*! 
	  \brief Checks if the component is enabled.
	  \return True of false depending on if the component is enabled.
	*/
	bool IsEnabled() {
		return m_enabled;
	}

	/*!
	  \brief Enable the component.
	*/
	void Enable() {
		m_enabled = true;
	}

	/*!
	  \brief Disable the component.
	*/
	void Disable() {
		m_enabled = false;
	}

	/*!
	  \brief Toggle the component on or off.
	*/
	void Toggle() {
		m_enabled = !m_enabled;
	}

	/*!
	  \brief Used to stream data from the component into SF packets.
	  \param packet The packet to stream data into.
	  \return The packet with the data in.
	*/

	virtual sf::Packet& Get(sf::Packet& packet) const {
		return packet;
	}

	/*!
	  \brief Used to stream data from SF packets into the component.
	  \param packet The packer to stream data from.
	  \param The updated packet.
	*/
	virtual sf::Packet& Set(sf::Packet& packet) {
		return packet;
	}

	/*!
	  \brief Used to call the get function and stream data into.
	  \param packet The packet containing the data.
	  \param c A shared pointer to the component.
	*/
	friend sf::Packet& operator<<(sf::Packet& packet, const std::shared_ptr<ComponentBase>& c) {
		return c->Get(packet);
	}

	/*!
	  \brief Used to call the set function and stream data into components.
	  \param packet The packet to stream data into.
	  \param c A shared pointer to the component.
	*/
	friend sf::Packet& operator>>(sf::Packet& packet, std::shared_ptr<ComponentBase>& c) {
		return c->Set(packet);
	}

	bool m_enabled; //!< True if this component is enabled.
};