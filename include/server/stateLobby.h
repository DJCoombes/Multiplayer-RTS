#pragma once

/**
* @Author	DJ Coombes
* @date		13th November 2016
* @brief	This state will handle the server lobby.
*/

#include "stateBase.h"

class StateLobby : public StateBase {
public:
	StateLobby(StateManager& stateManager);
	~StateLobby();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();
};