#pragma once

/**
* @Author	DJ Coombes
* @date		13th November 2016
* @brief	This is the state that will handle the game logic on the server.
*/

#include "stateBase.h"

#ifdef _DEBUG
#include <SFML/System/Clock.hpp>
#endif

#include "server.h"
#include "systemManager.h"

class StatePlaying : public StateBase {
public:
	/*!
	  \brief Constructor for the base state.
	  \param stateManager Reference to the state manager object.
	*/
	StatePlaying(StateManager& stateManager);

	/*!
	  \brief Virtual destructor.
	*/
	~StatePlaying();

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
private:
	EntityManager*					m_entityManager; //!< Pointer to the entity manager.
	std::shared_ptr<SystemManager>	m_systemManager; //!< Pointer to the system manager.
	int m_playerCount; //!< Amount of players that the game started with.
#ifdef _DEBUG
	sf::Clock m_clock; //!< Clock used for printing data usage every 30 seconds.
#endif
};