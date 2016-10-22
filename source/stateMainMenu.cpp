/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	This is the state containing information about the main menu.
*/

#include "stateMainMenu.h"

#include <iostream>

#include "stateManager.h"

StateMainMenu::StateMainMenu(StateManager& stateManager) : StateBase(stateManager) {

}

StateMainMenu::~StateMainMenu() {}

void StateMainMenu::OnCreate() {
	std::cout << "Main menu state created" << std::endl;

	m_stateManager.SwitchTo(StateType::PLAYING);
}

void StateMainMenu::OnDestroy() {}

void StateMainMenu::Update(const sf::Time& time) {}

void StateMainMenu::Draw() {}

void StateMainMenu::Activate() {}

void StateMainMenu::Deactivate() {}