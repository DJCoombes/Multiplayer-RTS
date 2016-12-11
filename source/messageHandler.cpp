/**
* @Author	DJ Coombes
* @date		21st October 2016
* @brief	Handle specific messages, allows messages to be dispatched to systems.
*/

#include "messageHandler.h"

void MessageHandler::Subscribe(EntityMessageType& type, Observer* observer) {
	m_communicators[type].Add(observer);
}

void MessageHandler::Unsubscribe(EntityMessageType& type, Observer* observer) {
	m_communicators[type].Remove(observer);
}

void MessageHandler::Dispatch(Message& message) {
	auto i = m_communicators.find((EntityMessageType)message.m_type);
	if (i == m_communicators.end())
		return;
	i->second.Broadcast(message);
}