#pragma once

/**
* @Author	DJ Coombes
* @date		21st October 2016
* @brief	Handles the queuing of events.
*/

#include <queue>

using EventID = unsigned int;

class EventQueue {
public:
	/*!
	  \brief Add an event to the queue.
	  \param event ID of the event.
	*/
	void AddEvent(const EventID event) {
		m_eventQueue.push(event);
	}

	/*!
	  \brief Process the queued event.
	  \param id ID of the event to process.
	  \return True if the event is found.
	*/
	bool ProcessEvents(EventID& id) {
		if (m_eventQueue.empty())
			return false;
		id = m_eventQueue.front();
		m_eventQueue.pop();
		return true;
	}

	/*!
	  \brief Clear all queued events.
	*/
	void Clear() {
		while (!m_eventQueue.empty())
			m_eventQueue.pop();
	}
private:
	std::queue<EventID> m_eventQueue; //!< Queue of the current events.
};