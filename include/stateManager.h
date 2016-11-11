#pragma once

/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	The state manager will handle all the available states.
*/

#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

#include "stateBase.h"
#include "sharedContext.h"

/*!
  \brief Available states.
*/
enum StateType {	
	INTRO,
	MAINMENU,
	LOADING, 
	PLAYING, 
	PAUSED 
};

using StateContainer = std::vector<std::pair<StateType, std::shared_ptr<StateBase>>>; //!< Container for storing state objects.
using OldStates = std::vector<StateType>; //!< Container for storing states that are to be removed.
using StateFactory = std::unordered_map<StateType, std::function<std::shared_ptr<StateBase>(void)>>; //!< Factory for creating state objects.

class StateManager {
public:
	/*!
	  \brief Constructor.
	  \param sharedContext Reference to the shared context object.
	*/
	StateManager(SharedContext& sharedContext);

	/*!
	  \brief Destructor.
	*/
	~StateManager();

	/*!
	  \brief Update available states.
	  \param time Time since last update.
	*/
	void Update(const sf::Time& time);

	/*!
	  \brief Draw available states.
	*/
	void Draw();

	/*!
	  \brief Delete states marked for deletion.
	*/
	void RemoveMarkedStates();

	/*!
	  \brief Get a reference to the shared context.
	  \return Reference to the shared context.
	*/
	SharedContext& GetContext();

	/*!
	  \brief Check if the manager currently has a certain state.
	  \param type Enum of the state you want to check.
	  \return True or false if the state exists.
	*/
	bool HasState(const StateType& type);

	/*!
	  \brief Switch to a certain state.
	  \param type Enum of the state you want to switch to.
	*/
	void SwitchTo(const StateType& type);

	/*!
	  \brief Mark a state for deletion.
	  \param type Enum of the state to remove.
	*/
	void Remove(const StateType& type);
private:

	/*!
	  \brief Create a certain state.
	  \param type Enum of the state to create.
	*/
	void CreateState(const StateType& type);

	/*!
	  \brief Delete state.
	  \param type Enum of the state to delete.
	*/
	void RemoveState(const StateType& type);

	/*!
	  \brief Lambda function for registering states via Enum.
	  \param type Enum type to register.
	*/

	template<typename T>
	void RegisterState(const StateType& type) {
		m_stateFactory[type] = [this]() -> std::shared_ptr<StateBase> {
			return std::make_shared<T>(*this);
		};
	}

	SharedContext&	m_context; //!< Reference to the shared context.
	StateContainer	m_states; //!< Container of all the current states.
	OldStates		m_toDelete; //!< Container of states to remove.
	StateFactory	m_stateFactory; //!< Function object of all states.

	std::map<StateType, std::string> m_stateStringMap; //!< Used for passing the new state type to the web UI.
};