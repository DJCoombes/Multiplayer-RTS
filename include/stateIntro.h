#pragma once

/**
* @Author	DJ Coombes
* @date		15th October 2016
* @brief	This state will show the introductory screen.
*/

#include "stateBase.h"

class StateIntro : public StateBase {
public:
	StateIntro(StateManager& stateManager);
	~StateIntro();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();
};