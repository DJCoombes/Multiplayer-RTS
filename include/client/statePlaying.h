#pragma once

/**
* @Author	DJ Coombes
* @date		22nd October 2016
* @brief	This is the state that contains all the information for playing the game.
*/

#include "stateBase.h"
#include "systemManager.h"
#include "packetTypes.h"
#include "client.h"

class StatePlaying : public StateBase {
public:
	StatePlaying(StateManager& stateManager);
	~StatePlaying();

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
	Window*							m_window; //!< Pointer to the window.
	EntityManager*					m_entityManager; //!< Pointer to the entity manager.
	std::shared_ptr<SystemManager>	m_systemManager; //!< Pointer to the system manager.
};