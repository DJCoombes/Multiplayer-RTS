#pragma once

/**
* @Author	DJ Coombes
* @date		04th November 2016
* @brief	The input forwarder will forward any SFLM input events to the web renderer.
*/

#include <map>

#include <SFML/Window/Event.hpp>
#include <Awesomium/WebCore.h>

class InputForwarder {
public:
	/*!
	  \brief Constructor.
	*/
	InputForwarder();

	/*!
	  \brief Send the event to the web renderer,
	  \param webView The web view to send the event to.
	  \param event The input event to pass to the web renderer.
	*/
	void HandleEvent(Awesomium::WebView* webView, const sf::Event& event);
private:
	std::map<sf::Keyboard::Key, int> m_keyIdentifierMap; //!< Map of SFML keyboard events to Awesomium events.
	std::map<sf::Keyboard::Key, int> m_keyAsciiMap; //!< Map of SFML keyboard events to ASCII.
};