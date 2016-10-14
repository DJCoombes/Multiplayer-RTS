#include "game.h"

#include <SFML/OpenGL.hpp>
#include <gl/GL.h>

Game::Game(std::shared_ptr<Window> window, int width, int height) {
	m_window = window;
}

Game::~Game() {}

void Game::ProcessEvents() {
	sf::Event event;

	while (m_window->GetWindow().pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			m_window->GetWindow().close();
	}
}

void Game::Update(sf::Time deltaTime) {

}

void Game::Render() {
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_window->GetWindow().display();
}