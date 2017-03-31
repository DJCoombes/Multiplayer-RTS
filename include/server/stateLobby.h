#pragma once

/**
* @Author	DJ Coombes
* @date		13th November 2016
* @brief	This state will handle the server lobby.
*/

#include "stateBase.h"

#include "server.h"

class StateLobby : public StateBase {
public:
	/*!
	  \brief Constructor for the base state.
	  \param stateManager Reference to the state manager object.
	*/
	StateLobby(StateManager& stateManager);

	/*!
	  \brief Virtual destructor.
	*/
	~StateLobby();

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
	  \brief Handles packets whilst in the lobby.
	  \param client ID of the client that sent the packet.
	  \param id ID OF the packet.
	  \param packet Packet that's been received.
	  \param server Pointer to the multi-player server.
	*/
	void HandlePacket(ClientID& client, PacketID& id, sf::Packet& packet, Server* server);
};