#pragma once

/**
* @Author	DJ Coombes
* @date		21st October 2016
* @brief	Communicator used for managing system messages.
*/

#include <algorithm>
#include <vector>

#include "observer.h"

using ObserverContainer = std::vector<Observer*>; //!< Container used for storing pointers to observers.

class Communicator {
public:
	/*!
	  \brief Add an observer.
	  \param observer Pointer to the observer to add.
	*/
	void Add(Observer* observer);

	/*!
	  \brief Remove an observer.
	  \param observer Pointer to observer to remove.
	*/
	void Remove(Observer* observer);

	/*!
	  \brief Check if we already have this observer.
	  \param observer Observer to check if we have.
	  \return True if we have the observer already.
	*/
	bool Has(Observer* observer);

	/*!
	  \brief Broadcast a message to the observers.
	  \param message Message to broadcast.
	*/
	void Broadcast(Message& message);
private:
	ObserverContainer m_observers; //!< Current observers.
};