/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	Main class for the whole game, contains the game loop.
*/

#include "game.h"

#include "gl_core_4_4.hpp"

Game::Game(Window& window) : m_window(window), 
	m_stateManager(m_sharedContext) {
	
	m_sharedContext.m_window = &window;

	m_stateManager.SwitchTo(StateType::INTRO);
}

Game::~Game() {}

void Game::ProcessEvents() {
	sf::Event event;

	while (m_window.GetWindow().pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			m_window.GetWindow().close();
	}
}

void Game::Update(sf::Time deltaTime) {
	m_entityManager.AddQueuedEntities();

	m_stateManager.Update(deltaTime);
	m_stateManager.RemoveMarkedStates();

	m_entityManager.DestroyQueuedEntities();
}

void Game::Render() {
	gl::ClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
	m_stateManager.Draw();

	m_window.GetWindow().display();
}