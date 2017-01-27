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
}

void StatePlaying::OnDestroy() {}

void StatePlaying::Update(const sf::Time& time) {
	if (!m_stateManager.GetContext().m_client->IsConnected())
		m_stateManager.SwitchTo(StateType::MAINMENU);

	// Add and destroy all the queued entities.
	m_entityManager->DestroyQueuedEntities();
	m_entityManager->AddQueuedEntities();

	m_systemManager->Update(time.asSeconds());
}

void StatePlaying::Draw() {
	m_systemManager->Draw();
}

void StatePlaying::Activate() {
	// Bind the function that's to be used as a packet handler.
	m_stateManager.GetContext().m_client->BindPacketHandler(&StatePlaying::HandlePacket, this);
}

void StatePlaying::Deactivate() {
	m_entityManager->Clear();
}

void StatePlaying::HandlePacket(PacketID& id, sf::Packet& packet, Client* client) {
	PacketType type = PacketType(id);

	if (type == PacketType::ENTITYCREATION) {
		std::string type;
		packet >> type;
		m_stateManager.GetContext().m_entityManager->Create(type);
	}
	else if (type == PacketType::ENTITYUPDATE) {
		auto mutex = &client->GetMutex();
		try {
			std::lock_guard<std::mutex> lock(*mutex);
		}
		catch (const std::exception& e) {
			LOG(DEBUG) << e.what();
		}
		EntityID id;
		packet >> id;
		auto entity = m_entityManager->GetEntity(id);
		if (entity == nullptr)
			return;
		packet >> *entity;
	}
}