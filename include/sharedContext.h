#pragma once

/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	This shared context will store any objects that need to be shared between states.
*/

#include "window.h"
#include "entityManager.h"

struct SharedContext {
	/*!
	  \brief Constructor.
	*/
	SharedContext() : m_window(nullptr),
		m_entityManager(nullptr) {}

	Window*			m_window; //!< Pointer to the context window.
	EntityManager*	m_entityManager; //!< Pointer to the entity manager.
};