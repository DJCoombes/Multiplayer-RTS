/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	This is the state containing information about the main menu.
*/

#include "stateMainMenu.h"

#include <iostream>

#include "stateManager.h"

StateMainMenu::StateMainMenu(StateManager& stateManager) : StateBase(stateManager),
	m_startLoading(false) {

}

StateMainMenu::~StateMainMenu() {}

void StateMainMenu::OnCreate() {
	std::cout << "Main menu state created" << std::endl;
	// Bind the C++ start and exit functions so they can be called from JavaScript.
	m_stateManager.GetContext().m_userInterface->BindMethod(Awesomium::WSLit("StartGame"), &StateMainMenu::Play, this);
	m_stateManager.GetContext().m_userInterface->BindMethod(Awesomium::WSLit("ExitGame"), &StateMainMenu::Exit, this);
}

void StateMainMenu::OnDestroy() {}

void StateMainMenu::Update(const sf::Time& time) {
	if (m_startLoading)
		m_stateManager.SwitchTo(StateType::LOADING);
}

void StateMainMenu::Draw() {}

void StateMainMenu::Activate() {
	m_startLoading = false;
}

void StateMainMenu::Deactivate() {}

void StateMainMenu::HandlePacket(PacketID& id, sf::Packet& packet, Client* client) {
	PacketType type = PacketType(id);

	if (type == PacketType::STARTLOADING) {
		m_startLoading = true;
	}
}

void StateMainMenu::Play(Awesomium::WebView* caller, const Awesomium::JSArray& args) {
	// Try to connect to the server on the local network.
	Client* client = m_stateManager.GetContext().m_client;
	sf::IpAddress ip("127.0.0.1");
	Port port = NetworkSpecifics::SERVERPORT;
	client->SetServerInfo(ip, port);
	client->BindPacketHandler(&StateMainMenu::HandlePacket, this);
	client->Connect();
}

void StateMainMenu::Exit(Awesomium::WebView* caller, const Awesomium::JSArray& args) {
	m_stateManager.GetContext().m_window->GetWindow().close();
}