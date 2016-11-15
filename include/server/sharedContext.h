#pragma once

/**
* @Author	DJ Coombes
* @date		11th November 2016
* @brief	This shared context will store any objects that need to be shared between states.
*/

#include "entityManager.h"
#include "server.h"

struct SharedContext {
	/*!
	  \brief Constructor.
	*/
	SharedContext() :
		m_entityManager(nullptr),
		m_server(nullptr)
	{}

	EntityManager*	m_entityManager; //!< Pointer to the entity manager.
	Server*			m_server; //!< Pointer to the server object.
};