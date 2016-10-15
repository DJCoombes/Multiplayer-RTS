#pragma once

/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	This is the state containing information about the main menu.
*/

#include "stateBase.h"

class StateMainMenu : public StateBase {
public:
	StateMainMenu(StateManager& stateManager);
	~StateMainMenu();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();
};