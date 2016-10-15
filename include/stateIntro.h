#pragma once

/**
* @Author	DJ Coombes
* @date		15th October 2016
* @brief	This state will show the introductory screen.
*/

#include "stateBase.h"

class StateIntro : public StateBase {
public:
	/*!
	  \brief Constructor.
	  \param stateManager Reference to the state manager.
	*/
	StateIntro(StateManager& stateManager);

	/*!
	  \brief Destructor.
	*/
	~StateIntro();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();
};