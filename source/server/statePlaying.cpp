/**
* @Author	DJ Coombes
* @date		13th November 2016
* @brief	This is the state that will handle the game logic on the server.
*/

#include "statePlaying.h"

#include "stateManager.h"

#include "componentMovement.h"

StatePlaying::StatePlaying(StateManager& stateManager) : StateBase(stateManager) {
	m_entityManager = m_stateManager.GetContext().m_entityManager;
}

StatePlaying::~StatePlaying() {
	m_entityManager->Clear();
}

void StatePlaying::OnCreate() {
	m_systemManager = std::make_shared<SystemManager>(m_stateManager.GetContext());
}

void StatePlaying::OnDestroy() {}

void StatePlaying::Update(const sf::Time& time) {
	m_entityManager->DestroyQueuedEntities();
	m_entityManager->AddQueuedEntities();

	m_systemManager->Update(time.asSeconds());

	auto server = m_stateManager.GetContext().m_server;

	for (auto& i : m_entityManager->GetEntities()) {
		sf::Packet packet;
		SetPacketType(PacketType::ENTITYUPDATE, packet);
		packet << *i;
		server->Broadcast(packet);
	}
}

void StatePlaying::Draw() {}

void StatePlaying::Activate() {
	m_stateManager.GetContext().m_server->BindPacketHandler(&StatePlaying::HandlePacket, this);
	int id = m_stateManager.GetContext().m_entityManager->Create("test");
	LOG(INFO) << id;
	auto temp = m_stateManager.GetContext().m_entityManager->GetEntity(id);
	auto mc = temp->Get<ComponentMovement>();
	mc->MoveTo(sf::Vector2f(100, 100));
}

void StatePlaying::Deactivate() {}

void StatePlaying::HandlePacket(ClientID& client, PacketID& id, sf::Packet& packet, Server* server) {
}