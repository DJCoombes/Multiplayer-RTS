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
	void Add(Observer* observer) {
		if (!Has(observer))
			m_observers.emplace_back(observer);
	}

	/*!
	  \brief Remove an observer.
	  \param observer Pointer to observer to remove.
	*/
	void Remove(Observer* observer) {
		auto temp = std::find(m_observers.begin(), m_observers.end(), observer);
		if (temp == m_observers.end())
			return;
		m_observers.erase(temp);
	}

	/*!
	  \brief Check if we already have this observer.
	  \param observer Observer to check if we have.
	  \return True if we have the observer already.
	*/
	bool Has(Observer* observer) {
		auto temp = std::find(m_observers.begin(), m_observers.end(), observer);
		if (temp == m_observers.end())
			return false;
		return true;
	}

	/*!
	  \brief Broadcast a message to the observers.
	  \param message Message to broadcast.
	*/
	void Broadcast(const Message& message) {
		for (auto& i : m_observers) { 
			i->Notify(message); 
		}
	}
private:
	ObserverContainer m_observers; //!< Current observers.
};