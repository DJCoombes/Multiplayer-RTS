/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	This is the state containing information about the main menu.
*/

#include "stateMainMenu.h"

#include <iostream>

#include "stateManager.h"

StateMainMenu::StateMainMenu(StateManager& stateManager) : StateBase(stateManager) {

}

StateMainMenu::~StateMainMenu() {}

void StateMainMenu::OnCreate() {
	std::cout << "Main menu state created" << std::endl;

	m_stateManager.GetContext().m_userInterface->BindMethod(Awesomium::WSLit("StartGame"), &StateMainMenu::Play, this);
	m_stateManager.GetContext().m_userInterface->BindMethod(Awesomium::WSLit("ExitGame"), &StateMainMenu::Exit, this);
}

void StateMainMenu::OnDestroy() {}

void StateMainMenu::Update(const sf::Time& time) {}

void StateMainMenu::Draw() {}

void StateMainMenu::Activate() {}

void StateMainMenu::Deactivate() {}

void StateMainMenu::HandlePacket(PacketID& id, sf::Packet& packet, Client* client) {

}

void StateMainMenu::Play(Awesomium::WebView* caller, const Awesomium::JSArray& args) {
	Client* client = m_stateManager.GetContext().m_client;
	sf::IpAddress ip("127.0.0.1");
	Port port = 5000;
	client->SetServerInfo(ip, port);
	client->Setup(&StateMainMenu::HandlePacket, this);
	client->Connect();
}

void StateMainMenu::Exit(Awesomium::WebView* caller, const Awesomium::JSArray& args) {
	m_stateManager.GetContext().m_window->GetWindow().close();
}