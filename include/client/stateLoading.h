#pragma once

/**
* @Author	DJ Coombes
* @date		18th November 2016
* @brief	This state will load any resources required to play the game.
*/

#include "stateBase.h"

#include "client.h"

class StateLoading : public StateBase {
public:
	/*!
	  \brief Constructor for the base state.
	  \param stateManager Reference to the state manager object.
	*/
	StateLoading(StateManager& stateManager);

	/*!
	  \brief Virtual destructor.
	*/
	~StateLoading();

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
private:
	bool m_startGame; //!< True if the state should be switch to playing, false otherwise.
};