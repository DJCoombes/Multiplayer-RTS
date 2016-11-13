#pragma once

/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	Main class for the whole game, contains the game loop.
*/

#include <memory>

#include "window.h"
#include "sharedContext.h"
#include "stateManager.h"
#include "entityManager.h"
#include "userInterface.h"
#include "client.h"

class Game {
public:
	/*!
	  \brief Game loop constructor.
	  \param window Reference to the context window.
	*/
	Game(Window& window);

	/*!
	  \brief Game loop destructor.
	*/
	~Game();

	/*!
	  \brief Process all user input events.
	*/
	void ProcessEvents();

	/*!
	  \brief Update the game loop.
	  \param deltaTime Time since the last update.
	*/
	void Update(sf::Time deltaTime);

	/*!
	  \brief Draw to the render buffer.
	*/
	void Render();

private:
	Window&			m_window; //!< Reference to the context window. 
	SharedContext	m_sharedContext; //!< Shared context containing any data to be shared.
	StateManager	m_stateManager; //!< Object that manages all the game states.
	EntityManager	m_entityManager; //!< Entity manager instance.
	UserInterface	m_userInterface; //!< User interface instance.
	Client			m_client; //!< Multiplayer client instance.
};