/**
* @Author	DJ Coombes
* @date		13th October 2016
* @brief	Main class for the application.
*/

#include <iostream>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

#include "game.h"
#include "logger.h"

int main(int argc, char* argv[]) 
{
	LOG::Clear();

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
		window.Display();
	}

	return 0;
}