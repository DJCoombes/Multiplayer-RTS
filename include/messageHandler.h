#pragma once

/**
* @Author	DJ Coombes
* @date		21st October 2016
* @brief	Handle specific messages, allows messages to be dispatched to systems.
*/

#include <unordered_map>

#include "communicator.h"
#include "entityMessages.h"

using Subscriptions = std::unordered_map<EntityMessage, Communicator>; //!< Map of subscriptions.

class MessageHandler {
public:
	/*!
	  \brief Subscribe an observer to a specific message.
	  \param type Type of message to subscribe to.
	  \param The Observer subscribing to the message.
	*/
	void Subscribe(const EntityMessage& type, Observer* observer) {
		m_communicators[type].Add(observer);
	}

	/*!
	  \brief Unsubscribe an observer from a specific message.
	  \param type Type of message to unsubscribe from.
	  \param observer Pointer to the observer to unsubscribe the message from.
	*/
	void Unsubscribe(const EntityMessage& type, Observer* observer) {
		m_communicators[type].Remove(observer);
	}

	/*!
	  \brief Dispatch a specific message to all listening observers.
	  \param message Message to dispatch.
	*/
	void Dispatch(const Message& message) {
		auto i = m_communicators.find((EntityMessage)message.m_type);
		if (i == m_communicators.end())
			return;
		i->second.Broadcast(message);
	}
private:
	Subscriptions m_communicators; //!< Map of messages to the subscribed communicators.
};