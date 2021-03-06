/**
* @Author	DJ Coombes
* @date		11th November 2016
* @brief	Main class for the whole game server, contains the game loop.
*/

#include "gameServer.h"

#include "luaFunctions.h"

GameServer::GameServer() : m_stateManager(m_sharedContext),
	m_running(true) {
	// Initialize the shared context pointers.
	m_sharedContext.m_entityManager = &m_entityManager;
	m_sharedContext.m_server = &m_server;
	m_entityManager.AddServerInstance(&m_server);

	LuaFuncs::manager = &m_entityManager;
	LuaFuncs::AddFunctions();

	m_stateManager.SwitchTo(StateType::LOBBY);
}

GameServer::~GameServer() {}

void GameServer::Update(sf::Time deltaTime) {
	m_server.Update(deltaTime);
	m_stateManager.Update(deltaTime);
	m_stateManager.RemoveMarkedStates();
}

bool GameServer::IsRunning() {
	return m_running;
}