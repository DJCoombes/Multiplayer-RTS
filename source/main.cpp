#include <iostream>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

#include "game.h"

int main(int argc, char* argv[]) 
{
	int width = 1280;
	int height = 720;

	std::shared_ptr<Window> window;
	window = std::make_shared<Window>(sf::VideoMode(width, height), std::string("Miltiplayer RTS"), sf::Style::Default);

	Game game(window, width, height);

	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (window->IsOpen()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;

		game.ProcessEvents();

		while (timeSinceLastUpdate >= timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;
			game.Update(timePerFrame);
		}

		game.Render();
		window->Display();
	}

	return 0;
}