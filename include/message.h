#pragma once

/**
* @Author	DJ Coombes
* @date		21 October 2016
* @brief	Messages used for system communication.
*/

using MessageType = unsigned int; //!< Type of message;

//!< Message with the information.
struct Message {
	/*!
	  \brief Constructor.
	  \param type Type of the message.
	*/
	Message(const MessageType& type) : m_type(type) {}

	MessageType m_type; //!< Message type.
};