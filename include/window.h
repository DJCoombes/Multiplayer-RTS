#pragma once

/**
 * @Author	DJ Coombes
 * @date	13th October 2016
 * @brief	Wrapper class for the SFML context window.
 */

#include <string>

#include <SFML/Graphics.hpp>

class Window {
public:
	/*!
	  \brief Window constructor.
	  \param mode Dimensions for the context window.
	  \param title Title to display on the context window.
	  \param style Style of the window(Fullscreen, windowed).
	*/
	Window(sf::VideoMode mode, std::string& title, sf::Uint32 style);
	
	/*!
	  \brief Window destructor.
	*/
	~Window();
	
	/*!
	  \brief Set the vertical synchronization value.
	  \param enabled Set the value for Vsync.
	*/
	void SetVsync(bool enabled);

	/*!
	  \brief Check if the window is open.
	  \return If the window is open.
	*/
	bool IsOpen();

	/*!
	  \brief Return a reference to the context window.
	  \return Reference to the context window.
	*/
	sf::RenderWindow& GetWindow();

private:
	sf::RenderWindow m_sfWindow; //!< Instance of the context window.

	int			m_width; //!< Width of the context window.
	int			m_height; //!< Height of the context window.
	sf::Uint32	m_currentStyle; //!< Style of the context window.
	std::string	m_title; //!< Title of the context window.
};