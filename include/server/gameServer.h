#pragma once

/**
* @Author	DJ Coombes
* @date		11th November 2016
* @brief	Main class for the whole game server, contains the game loop.
*/

#include <SFML/System/Time.hpp>

#include "sharedContext.h"
#include "stateManager.h"
#include "entityManager.h"
#include "server.h"

class GameServer {
public:
	/*!
	  \brief Constructor.
	*/
	GameServer();

	/*!
	  \brief Destructor.
	*/
	~GameServer();

	/*!
	  \brief Update the game server loop.
	  \param deltaTime Time since the last update.
	*/
	void Update(sf::Time deltaTime);

	/*!
	  \brief Checks if the game server is running.
	  \return True if the server is running, false otherwise.
	*/
	bool IsRunning();
private:
	bool			m_running; // True if the server is running, false otherwise.
	SharedContext	m_sharedContext; //!< Shared context containing any data to be shared.
	StateManager	m_stateManager; //!< Object that manages all the game states.
	EntityManager	m_entityManager; //!< Entity manager instance.
	Server			m_server; //!< The server object that handles connections.
};