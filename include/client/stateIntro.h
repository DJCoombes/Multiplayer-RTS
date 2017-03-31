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
	  \brief Constructor for the base state.
	  \param stateManager Reference to the state manager object.
	*/
	StateIntro(StateManager& stateManager);

	/*!
	  \brief Virtual destructor.
	*/
	~StateIntro();

	/*!
	  \brief initialize state information / variables.
	*/
	void OnCreate();

	/*!
	  \brief Clean up the state.
	*/
	void OnDestroy();

	/*!
	  \brief Activate this state.
	*/
	void Activate();

	/*!
	  \brief Deactivate this state.
	*/
	void Deactivate();

	/*!
	  \brief Update the current state.
	  \param time Time passed since last update.
	*/
	void Update(const sf::Time& time);

	/*!
	  \brief Draw the current state.
	*/
	void Draw();
};