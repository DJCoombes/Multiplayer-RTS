#pragma once

/**
* @Author	DJ Coombes
* @date		21 October 2016
* @brief	Observer used for listening for messages.
*/

#include "message.h"

class Observer {
public:
	/*!
	  \brief Virtual destructor.
	*/
	virtual ~Observer() {}

	/*!
	  \brief Used for sending messages needs to be implemented in derived classes.
	  \param message Message to send.
	*/
	virtual void Notify(const Message& message) = 0;
};