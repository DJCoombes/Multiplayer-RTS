/**
* @Author	DJ Coombes
* @date		13th November 2016
* @brief	This is the state that will handle the game logic on the server.
*/

#include "statePlaying.h"

#include "stateManager.h"

StatePlaying::StatePlaying(StateManager& stateManager) : StateBase(stateManager) {
	m_stateManager.GetContext().m_server->BindPacketHandler(&StatePlaying::HandlePacket, this);
}

StatePlaying::~StatePlaying() {}

void StatePlaying::OnCreate() {
}

void StatePlaying::OnDestroy() {}

void StatePlaying::Update(const sf::Time& time) {}

void StatePlaying::Draw() {}

void StatePlaying::Activate() {
	m_stateManager.GetContext().m_entityManager->Create("test");
}

void StatePlaying::Deactivate() {}

void StatePlaying::HandlePacket(ClientID& client, PacketID& id, sf::Packet& packet, Server* server) {
}