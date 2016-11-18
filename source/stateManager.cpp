/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	The state manager will handle all the available states.
*/

#include "stateManager.h"
#ifdef GAME
#include "stateIntro.h"
#include "stateMainMenu.h"
#include "stateLoading.h"
#include "statePlaying.h"
#elif defined SERVER
#include "stateLobby.h"
#include "statePlaying.h"
#endif

StateManager::StateManager(SharedContext& sharedContext) : m_context(sharedContext) {
#ifdef GAME
	RegisterState<StateIntro>(StateType::INTRO);
	RegisterState<StateMainMenu>(StateType::MAINMENU);
	RegisterState<StateLoading>(StateType::LOADING);
	RegisterState<StatePlaying>(StateType::PLAYING);

	m_stateStringMap[StateType::INTRO]		= "intro";
	m_stateStringMap[StateType::MAINMENU]	= "mainMenu";
	m_stateStringMap[StateType::LOADING]	= "loading";
	m_stateStringMap[StateType::PLAYING]	= "playing";
#elif SERVER
	RegisterState<StateLobby>(StateType::LOBBY);
	RegisterState<StatePlaying>(StateType::PLAYING);

	m_stateStringMap[StateType::LOBBY] = "lobby";
	m_stateStringMap[StateType::PLAYING] = "playing";
#endif
}

StateManager::~StateManager() {

}

void StateManager::Update(const sf::Time& time) {
	if (m_states.empty())
		return;
	if (m_states.back().second->IsTranscendent() && m_states.size() > 1) {
		// Go backwards through all the states until we hit one that isn't transcendent.
		auto i = m_states.end();
		while (i != m_states.begin()) {
			if (i != m_states.end()) {
				if (!i->second->IsTranscendent())
					break;
			}
			--i;
		}
		// Now go through all the transcendent states and update them.
		for (; i != m_states.end(); i++)
			i->second->Update(time);
	}
	else {
		m_states.back().second->Update(time);
	}
}
#ifdef GAME
void StateManager::Draw() {
	if (m_states.empty())
		return;
	if (m_states.back().second->IsTransparent() && m_states.size() > 1) {
		// Go backwards through all the states until we hit one that isn't transparent.
		auto i = m_states.end();
		while (i != m_states.begin()) {
			if (i != m_states.end()) {
				if (!i->second->IsTransparent())
					break;
			}
			--i;
		}
		// Now go through all the transparent states and draw them.
		for (; i != m_states.end(); ++i)
			i->second->Draw();
	}
	else {
		m_states.back().second->Draw();
	}
}
#endif
SharedContext& StateManager::GetContext() {
	return m_context;
}

bool StateManager::HasState(const StateType& type) {
	for (auto i : m_states) {
		if (i.first == type) {
			auto removed = std::find(m_toDelete.begin(), m_toDelete.end(), type);
			if (removed == m_toDelete.end())
				return true;
			return false;
		}
	}
	return false;
}

void StateManager::RemoveMarkedStates() {
	while (m_toDelete.begin() != m_toDelete.end()) {
		StateType temp = *m_toDelete.begin();
		RemoveState(*m_toDelete.begin());
		m_toDelete.erase(m_toDelete.begin());
	}
}

void StateManager::SwitchTo(const StateType& type) {
#ifdef GAME
	Awesomium::JSArray args;
	Awesomium::JSValue newState = Awesomium::WSLit(m_stateStringMap[type].c_str());
	args.Push(newState);
	m_context.m_userInterface->CallJSFunc(Awesomium::WSLit("SwitchState"), args);
#endif
	for (auto i = m_states.begin(); i != m_states.end(); ++i) {
		if (i->first == type) {
			m_states.back().second->Deactivate();
			StateType tempType = i->first;
			std::shared_ptr<StateBase> tempState = i->second;
			m_states.erase(i);
			m_states.emplace_back(tempType, tempState);
			tempState->Activate();
			return;
		}
	}

	if (!m_states.empty())
		m_states.back().second->Deactivate();
	CreateState(type);
	m_states.back().second->Activate();
}

void StateManager::Remove(const StateType& type) {
	m_toDelete.push_back(type);
}

void StateManager::CreateState(const StateType& type) {
	auto newState = m_stateFactory.find(type);
	if (newState == m_stateFactory.end())
		return;
	auto state = newState->second();
	m_states.emplace_back(type, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType& type) {
	for (auto i = m_states.begin(); i != m_states.end(); ++i) {
		if (i->first == type) {
			i->second->OnDestroy();
			m_states.erase(i);
			return;
		}
	}
}