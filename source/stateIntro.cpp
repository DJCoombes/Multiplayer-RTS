/**
* @Author	DJ Coombes
* @date		15th October 2016
* @brief	This state will show the introductory screen.
*/

#include "stateIntro.h"

#include <iostream>

#include "stateManager.h"

StateIntro::StateIntro(StateManager& stateManager) : StateBase(stateManager) {

}

StateIntro::~StateIntro() {}

void StateIntro::OnCreate() {
	std::cout << "Into state created" << std::endl;

	m_stateManager.SwitchTo(StateType::MAINMENU);
}

void StateIntro::OnDestroy() {}

void StateIntro::Update(const sf::Time& time) {}

void StateIntro::Draw() {}

void StateIntro::Activate() {}

void StateIntro::Deactivate() {}