#pragma once

/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	This is the state containing information about the main menu.
*/

#include "stateBase.h"

#include <Awesomium/WebCore.h>

#include "packetTypes.h"
#include "client.h"

class StateMainMenu : public StateBase {
public:
	/*!
	  \brief Constructor for the base state.
	  \param stateManager Reference to the state manager object.
	*/
	StateMainMenu(StateManager& stateManager);

	/*!
	  \brief Virtual destructor.
	*/
	~StateMainMenu();

	/*!
	  \brief initialize state information / variables.
	*/
	void OnCreate();

	/*!
	  \brief Clean up the state.
	*/
	void OnDestroy();

	/*!
	  \brief Activate this state.
	*/
	void Activate();

	/*!
	  \brief Deactivate this state.
	*/
	void Deactivate();

	/*!
	  \brief Update the current state.
	  \param time Time passed since last update.
	*/
	void Update(const sf::Time& time);

	/*!
	  \brief Draw the current state.
	*/
	void Draw();

	/*!
	  \brief Handles packets whilst in the main menu.
	  \param id ID OF the packet.
	  \param packet Packet that's been received.
	  \param client Pointer to the multi-player client.
	*/
	void HandlePacket(PacketID& id, sf::Packet& packet, Client* client);

	/*!
	  \brief Called from the user interface, connects to the game server.
	  \param caller The web view that called the function.
	  \param args The arguments passed by the web view.
	*/
	void Play(Awesomium::WebView* caller, const Awesomium::JSArray& args);

	/*!
	  \brief Called from the user interface, exits the game.
	  \param caller The web view that called the function.
	  \param args The arguments passed by the web view.
	*/
	void Exit(Awesomium::WebView* caller, const Awesomium::JSArray& args);
private:
	bool m_startLoading; //!< Used to check if the state should be switched to playing.
};