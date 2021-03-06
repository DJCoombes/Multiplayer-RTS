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
#ifdef _DEBUG
	m_stateManager.GetContext().m_userInterface->CallJSFunc(Awesomium::WSLit("ShowDebug"), Awesomium::JSArray());
#endif
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

	{
		std::lock_guard<std::mutex> lock(m_stateManager.GetContext().m_client->GetMutex());
		// Add and destroy all the queued entities.
		m_entityManager->DestroyQueuedEntities();
		m_entityManager->AddQueuedEntities();
	}

	m_systemManager->Update(time.asSeconds());
#ifdef _DEBUG
	int dataReceived = m_stateManager.GetContext().m_client->GetDataReceieved() / 1000;
	int dataSent = m_stateManager.GetContext().m_client->GetDataSent() / 1000;
	Awesomium::JSArray args;
	Awesomium::JSValue received(dataReceived);
	Awesomium::JSValue sent(dataSent);
	args.Push(received);
	args.Push(sent);
	m_stateManager.GetContext().m_userInterface->CallJSFunc(Awesomium::WSLit("UpdateData"), args);
#endif
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
	std::lock_guard<std::mutex> lock(client->GetMutex());
	PacketType type = PacketType(id);

	if (type == PacketType::ENTITYCREATION) {
		std::string type;
		packet >> type;
		int id;
		packet >> id;
		m_stateManager.GetContext().m_entityManager->Create(type, id);
	}
	else if (type == PacketType::ENTITYUPDATE) {		
		EntityID id;
		packet >> id;
		auto entity = m_entityManager->GetEntity(id);
		if (entity == nullptr)
			return;
		packet >> *entity;
	}
	else if (type == PacketType::ENTITYDESTRUCTION) {
		int id;
		packet >> id;
		m_stateManager.GetContext().m_entityManager->Destroy(id);
	}
}