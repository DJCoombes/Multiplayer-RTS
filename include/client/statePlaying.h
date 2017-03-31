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
	/*!
	  \brief Constructor for the base state.
	  \param stateManager Reference to the state manager object.
	*/
	StatePlaying(StateManager& stateManager);

	/*!
	  \brief Destructor.
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
	  \brief Handles packets whilst in the main menu.
	  \param id ID OF the packet.
	  \param packet Packet that's been received.
	  \param client Pointer to the multi-player client.
	*/
	void HandlePacket(PacketID& id, sf::Packet& packet, Client* client);
private:
	Window*							m_window; //!< Pointer to the window.
	EntityManager*					m_entityManager; //!< Pointer to the entity manager.
	std::shared_ptr<SystemManager>	m_systemManager; //!< Pointer to the system manager.
};