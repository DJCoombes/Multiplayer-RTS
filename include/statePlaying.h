#pragma once

/**
* @Author	DJ Coombes
* @date		22nd October 2016
* @brief	This is the state that contains all the information for playing the game.
*/

#include "stateBase.h"
#include "systemManager.h"

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
private:
	Window*							m_window; //!< Pointer to the window.
	EntityManager*					m_entityManager; //!< Pointer to the entity manager.
	std::shared_ptr<SystemManager>	m_systemManager; //!< Pointer to the system manager.
};