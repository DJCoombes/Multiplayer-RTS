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
	StateMainMenu(StateManager& stateManager);
	~StateMainMenu();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	/*!
	  \brief Handles packets whilst in the main menu.
	  \param id ID OF the packet.
	  \param packet Packet that's been received.
	  \param client Pointer to the multiplayer client.
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
};