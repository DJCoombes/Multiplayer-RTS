/**
* @Author	DJ Coombes
* @date		11th November 2016
* @brief	Main class for the game server.
*/

#include <SFML/System.hpp>

#include "logger.h"
#include "gameServer.h"

int main(int argc, char* argv[]) {
	// Clear the log file.
	LOG::Clear();

	GameServer gameServer;

	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	bool running = true;

	while (gameServer.IsRunning()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate >= timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;
			gameServer.Update(timePerFrame);
		}
	}
	
	return 0;
}