/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	Main class for the whole game, contains the game loop.
*/

#include "game.h"

#include "gl.hpp"

Game::Game(Window& window) : m_window(window), 
	m_stateManager(m_sharedContext) ,
	m_userInterface(window) {
	// Initialize all the shared context pointers.
	m_sharedContext.m_window = &window;
	m_sharedContext.m_entityManager = &m_entityManager;
	m_sharedContext.m_userInterface = &m_userInterface;
	m_sharedContext.m_client = &m_client;

	m_userInterface.InitializeUI();
	// Switch to the intro state.
	m_stateManager.SwitchTo(StateType::INTRO);
}

Game::~Game() {}

void Game::ProcessEvents() {
	sf::Event event;

	while (m_window.GetWindow().pollEvent(event)) {
		m_userInterface.HandleEvent(event);

		if (event.type == sf::Event::Closed)
			m_window.GetWindow().close();
	}
}

void Game::Update(sf::Time deltaTime) {
	m_client.Update(deltaTime);
	m_stateManager.Update(deltaTime);
	m_stateManager.RemoveMarkedStates();
}

void Game::Render() {
	// Load any changes from the web renderer.
	m_userInterface.UpdateView();

	if (!m_window.IsOpen())
		return;
	// Clear the window with white.
	gl::ClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
	m_stateManager.Draw();
	m_userInterface.DrawUI();
}