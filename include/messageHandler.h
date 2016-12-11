#pragma once

/**
* @Author	DJ Coombes
* @date		21st October 2016
* @brief	Handle specific messages, allows messages to be dispatched to systems.
*/

#include <unordered_map>

#include "communicator.h"
#include "entityMessages.h"

using SubscriptionsContainer = std::unordered_map<EntityMessageType, Communicator>; //!< Map of subscriptions.

class MessageHandler {
public:
	/*!
	  \brief Subscribe an observer to a specific message.
	  \param type Type of message to subscribe to.
	  \param The Observer subscribing to the message.
	*/
	void Subscribe(EntityMessageType& type, Observer* observer);

	/*!
	  \brief Unsubscribe an observer from a specific message.
	  \param type Type of message to unsubscribe from.
	  \param observer Pointer to the observer to unsubscribe the message from.
	*/
	void Unsubscribe(EntityMessageType& type, Observer* observer);

	/*!
	  \brief Dispatch a specific message to all listening observers.
	  \param message Message to dispatch.
	*/
	void Dispatch(Message& message);
private:
	SubscriptionsContainer m_communicators; //!< Map of messages to the subscribed communicators.
};