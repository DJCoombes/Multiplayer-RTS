#pragma once

/**
* @Author	DJ Coombes
* @date		13th November 2016
* @brief	This is the state that will handle the game logic on the server.
*/

#include "stateBase.h"

class StatePlaying : public StateBase {
public:
	StatePlaying(StateManager& stateManager);
	~StatePlaying();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();
};