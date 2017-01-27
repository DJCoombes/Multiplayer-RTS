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
	// Add and delete the queued entities.
	m_entityManager->DestroyQueuedEntities();
	m_entityManager->AddQueuedEntities();

	m_systemManager->Update(time.asSeconds());

	auto server = m_stateManager.GetContext().m_server;
	if (server->AmountOfClients() < m_playerCount) {
		server->DisconnectAll();
		m_stateManager.SwitchTo(StateType::LOBBY);
	}

	// Broadcast all the entities data to the clients.
	for (auto& i : m_entityManager->GetEntities()) {
		sf::Packet packet;
		SetPacketType(PacketType::ENTITYUPDATE, packet);
		packet << *i;
		server->Broadcast(packet); 
	}
}

void StatePlaying::Draw() {}

void StatePlaying::Activate() {
	// Bind the packet handler.
	m_stateManager.GetContext().m_server->BindPacketHandler(&StatePlaying::HandlePacket, this);
	// Create 2 test entities for the prototype.
	int id = m_stateManager.GetContext().m_entityManager->Create("test");
	LOG(INFO) << id;
	auto temp = m_stateManager.GetContext().m_entityManager->GetEntity(id);
	auto mc = temp->Get<ComponentMovement>();
	mc->MoveTo(sf::Vector2f(100, 100));

	int id2 = m_stateManager.GetContext().m_entityManager->Create("test");
	LOG(INFO) << id2;
	auto temp2 = m_stateManager.GetContext().m_entityManager->GetEntity(id2);
	auto mc2 = temp2->Get<ComponentMovement>();
	mc2->MoveTo(sf::Vector2f(200, 200));

	auto server = m_stateManager.GetContext().m_server;
	m_playerCount = server->AmountOfClients();
}

void StatePlaying::Deactivate() {
	m_entityManager->Clear();
}

void StatePlaying::HandlePacket(ClientID& client, PacketID& id, sf::Packet& packet, Server* server) {
	PacketType type = PacketType(id);

	if (type == PacketType::MOVEORDER) {
		EntityID id;
		int x, y;
		packet >> id >> x >> y;

		auto mutex = &server->GetMutex();
		try {
			std::lock_guard<std::mutex> lock(*mutex);
		}
		catch (const std::exception& e) {
			LOG(DEBUG) << e.what();
		}

		auto entity = m_entityManager->GetEntity(id);
		if (entity != nullptr) {
			auto mc = entity->Get<ComponentMovement>();
			if (mc != nullptr) {
				mc->MoveTo(sf::Vector2f((float)x, (float)y));
			}
		}
	}
}