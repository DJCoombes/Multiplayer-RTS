/**
* @Author	DJ Coombes
* @date		13th November 2016
* @brief	This is the state that will handle the game logic on the server.
*/

#include "statePlaying.h"

#include "stateManager.h"

StatePlaying::StatePlaying(StateManager& stateManager) : StateBase(stateManager) {

}

StatePlaying::~StatePlaying() {}

void StatePlaying::OnCreate() {}

void StatePlaying::OnDestroy() {}

void StatePlaying::Update(const sf::Time& time) {}

void StatePlaying::Draw() {}

void StatePlaying::Activate() {}

void StatePlaying::Deactivate() {}