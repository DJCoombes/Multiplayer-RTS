#pragma once

/**
* @Author	DJ Coombes
* @date		21 October 2016
* @brief	Messages used for system communication.
*/

using MessageType = unsigned int; //!< Type of message;

//!< Used to store two float worth of information.
struct TwoFloats {
	float m_x;
	float m_y;
};

//!< Message with the information.
struct Message {
	/*!
	  \brief Constructor.
	  \param type Type of the message.
	*/
	Message(const MessageType& type) : m_type(type) {}

	MessageType m_type; //!< Message type.
	int m_sender; //!< System that sent the message.
	int m_receiver; //!< System to receive the message.

	union {
		TwoFloats m_2f;
		bool m_bool;
		int m_int;
	};
};