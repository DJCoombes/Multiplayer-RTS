/**
* @Author	DJ Coombes
* @date		18th November 2016
* @brief	This state will load any resources required to play the game.
*/


#include "stateLoading.h"

#include <iostream>

#include "stateManager.h"

StateLoading::StateLoading(StateManager& stateManager) : StateBase(stateManager) {

}

StateLoading::~StateLoading() {}

void StateLoading::OnCreate() {
	std::cout << "Loading state created" << std::endl;
}

void StateLoading::OnDestroy() {}

void StateLoading::Update(const sf::Time& time) {
	if (m_startGame)
		m_stateManager.SwitchTo(StateType::PLAYING);
}

void StateLoading::Draw() {}

void StateLoading::Activate() {
	m_startGame = false;
	// Bind the function to be used as a packet handler.
	m_stateManager.GetContext().m_client->BindPacketHandler(&StateLoading::HandlePacket, this);
	// Tell the server that we've finished loading assets.
	sf::Packet packet;
	SetPacketType(PacketType::LOADINGCOMPLETE, packet);
	m_stateManager.GetContext().m_client->Send(packet);
}

void StateLoading::Deactivate() {}

void StateLoading::HandlePacket(PacketID& id, sf::Packet& packet, Client* client) {
	PacketType type = PacketType(id);

	if (type == PacketType::START) {
		m_startGame = true;
	}
	else if (type == PacketType::ENTITYCREATION) {
		std::string type;
		packet >> type;
		int id;
		packet >> id;
		m_stateManager.GetContext().m_entityManager->Create(type, id);
	}
	else if (type == PacketType::ENTITYDESTRUCTION) {
		int id;
		packet >> id;
		m_stateManager.GetContext().m_entityManager->Destroy(id);
	}
}