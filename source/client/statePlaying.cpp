/**
* @Author	DJ Coombes
* @date		22nd October 2016
* @brief	This is the state that contains all the information for playing the game.
*/

#include "statePlaying.h"

#include "stateManager.h"

StatePlaying::StatePlaying(StateManager& stateManager) : StateBase(stateManager) {
	m_window = m_stateManager.GetContext().m_window;
	m_entityManager = m_stateManager.GetContext().m_entityManager;
}

StatePlaying::~StatePlaying() {
	m_entityManager->Clear();
}

void StatePlaying::OnCreate() {
	m_systemManager = std::make_unique<SystemManager>(m_stateManager.GetContext());
	m_stateManager.GetContext().m_client->BindPacketHandler(&StatePlaying::HandlePacket, this);
}

void StatePlaying::OnDestroy() {}

void StatePlaying::Update(const sf::Time& time) {
	m_entityManager->DestroyQueuedEntities();
	m_entityManager->AddQueuedEntities();

	m_systemManager->Update(time.asSeconds());
}

void StatePlaying::Draw() {
	m_systemManager->Draw();
}

void StatePlaying::Activate() {}

void StatePlaying::Deactivate() {}

void StatePlaying::HandlePacket(PacketID& id, sf::Packet& packet, Client* client) {

}