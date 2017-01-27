/**
* @Author	DJ Coombes
* @date		13th November 2016
* @brief	This state will handle the server lobby.
*/

#include "stateLobby.h"

#include "stateManager.h"

StateLobby::StateLobby(StateManager& stateManager) : StateBase(stateManager) {}

StateLobby::~StateLobby() {}

void StateLobby::OnCreate() {
	Server* server = m_stateManager.GetContext().m_server;
	if (!server->Start()) {
		LOG(ERRORR) << "Error starting the sever.";
	}
}

void StateLobby::OnDestroy() {}

void StateLobby::Update(const sf::Time& time) {
	Server* server = m_stateManager.GetContext().m_server;
	// Tell the clients to start loading.
	if (server->ClientsReady()) {
		sf::Packet packet;
		SetPacketType(PacketType::STARTLOADING, packet);
		server->Broadcast(packet);
	}
	// Tell the clients to switch to the playing state.
	if (server->ClientsLoaded()) {
		sf::Packet packet;
		SetPacketType(PacketType::START, packet);
		server->Broadcast(packet);
		m_stateManager.SwitchTo(StateType::PLAYING);
	}
}

void StateLobby::Draw() {}

void StateLobby::Activate() {
	Server* server = m_stateManager.GetContext().m_server;

	// Bind the function used to handle the packets.
	server->BindPacketHandler(&StateLobby::HandlePacket, this);
}

void StateLobby::Deactivate() {}

void StateLobby::HandlePacket(ClientID& client, PacketID& id, sf::Packet& packet, Server* server) {
	LOG(INFO) << "PACKET RECEIVED";
}