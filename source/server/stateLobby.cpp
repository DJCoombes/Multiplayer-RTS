/**
* @Author	DJ Coombes
* @date		13th November 2016
* @brief	This state will handle the server lobby.
*/

#include "stateLobby.h"

#include "stateManager.h"

StateLobby::StateLobby(StateManager& stateManager) : StateBase(stateManager) {
	Server* server = stateManager.GetContext().m_server;
	if (!server->Start()) {
		LOG(ERRORR) << "Error starting the sever.";
	}
	server->BindPacketHandler(&StateLobby::HandlePacket, this);
}

StateLobby::~StateLobby() {}

void StateLobby::OnCreate() {}

void StateLobby::OnDestroy() {}

void StateLobby::Update(const sf::Time& time) {
	Server* server = m_stateManager.GetContext().m_server;

	if (server->ClientsReady()) {
		sf::Packet packet;
		SetPacketType(PacketType::START, packet);
		server->Broadcast(packet);
		m_stateManager.SwitchTo(StateType::PLAYING);
	}
}

void StateLobby::Draw() {}

void StateLobby::Activate() {}

void StateLobby::Deactivate() {}

void StateLobby::HandlePacket(ClientID& client, PacketID& id, sf::Packet& packet, Server* server) {
	LOG(INFO) << "PACKET RECEIVED";
}