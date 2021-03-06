#pragma once

/**
 * @Author	DJ Coombes
 * @date	13th October 2016
 * @brief	Wrapper class for the SFML context window.
 */

#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Window {
public:
	/*!
	  \brief Window constructor.
	  \param mode Dimensions for the context window.
	  \param title Title to display on the context window.
	  \param style Style of the window(Fullscreen, windowed).
	*/
	Window(sf::VideoMode mode, const std::string& title, sf::Uint32 style);
	
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

	/*!
	  \brief Get the width of the window.
	  \return Int containing the width.
	*/
	int GetWidth();

	/*!
	  \brief Get the height of the window.
	  \return Int containing the height.
	*/
	int GetHeight();

	/*!
	  \brief Output the render buffer to the window.
	*/
	void Display();
private:
	sf::RenderWindow m_sfWindow; //!< Instance of the context window.

	int			m_width; //!< Width of the context window.
	int			m_height; //!< Height of the context window.
	sf::Uint32	m_currentStyle; //!< Style of the context window.
	std::string	m_title; //!< Title of the context window.
};