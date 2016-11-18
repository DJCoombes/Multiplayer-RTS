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
	StateLoading(StateManager& stateManager);
	~StateLoading();

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
private:
	bool m_startGame; //!< True if the state should be switch to playing, false otherwise.
};