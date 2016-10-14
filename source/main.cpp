#include <iostream>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

#include "game.h"

int main(int argc, char* argv[]) 
{
	int width = 1280;
	int height = 720;

	Window window(sf::VideoMode(width, height), "Multiplayer RTS", sf::Style::Default);

	Game game(window);

	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (window.IsOpen()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;

		game.ProcessEvents();

		while (timeSinceLastUpdate >= timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;
			game.Update(timePerFrame);
		}

		game.Render();
		window.GetWindow().display();
	}

	return 0;
}