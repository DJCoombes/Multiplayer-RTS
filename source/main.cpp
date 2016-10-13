#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "window.h"

int main(int argc, char* argv[]) 
{
	int width = 1280;
	int height = 720;

	Window window(sf::VideoMode(width, height), std::string("Miltiplayer RTS"), sf::Style::Default);

	glEnable(GL_TEXTURE_2D);

	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (window.IsOpen()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate >= timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;
		}

		window.Display();
	}

	return 0;
}