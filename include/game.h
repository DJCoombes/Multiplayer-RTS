#pragma once

/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	Main class for the whole game, contains the game loop.
*/

#include <memory>

#include "window.h"

class Game {
public:
	/*!
	  \brief Game loop constructor.
	  \param window Reference to the context window.
	  \param width Width of the context window.
	  \param height of the context window.
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
	Window& m_window; //!< Reference to the context window. 
};