#pragma once

/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	Base class for all states. Functionality must be implemented in inherited classes.
*
* This is the base class for all game states, these classes can be transcendent and / or transparent meaning
* other states can update over them and / or draw over them.
*/

#include <SFML/System/Time.hpp>

class StateManager;

class StateBase {
	friend class StateManager;
public:
	/*!
	  \brief Constructor for the base state.
	  \param stateManager Reference to the state manager object.
	*/
	StateBase(StateManager& stateManager) : 
		m_stateManager(stateManager),
		m_transparent(false),
		m_transcendent(false) {}

	/*!
	  \brief Virtual destructor.
	*/
	virtual ~StateBase() {}

	/*!
	  \brief initialize state information / variables.
	*/
	virtual void OnCreate() = 0;

	/*!
	  \brief Clean up the state.
	*/
	virtual void OnDestroy() = 0;

	/*!
	  \brief Activate this state.
	*/
	virtual void Activate() = 0;

	/*!
	  \brief Deactivate this state.
	*/
	virtual void Deactivate() = 0;

	/*!
	  \brief Update the current state.
	  \param time Time passed since last update.
	*/
	virtual void Update(const sf::Time& time) = 0;

	/*!
	  \brief Draw the current state.
	*/
	virtual void Draw() = 0;

	/*!
	  \brief Set the current state to transparent.
	  \param transparent True or false.
	*/
	void SetTransparent(const bool & transparent) {
		m_transparent = transparent;
	}

	/*!
	  \brief Check if the state is transparent.
	  \return True or false.
	*/
	bool IsTransparent() const {
		return m_transparent;
	}

	/*!
	  \brief Set the state to transcendent.
	  \param transcendent True or false;
	*/
	void SetTranscendent(const bool& transcendent) {
		m_transcendent = transcendent;
	}

	/*!
	  \brief Check if the state is transcendent.
	  \return True or false.
	*/
	bool IsTranscendent() const {
		return m_transcendent;
	}

	/*!
	  \brief Get a reference to the state manager.
	  \return Reference to the state manager.
	*/
	StateManager& GetStateManager() {
		return m_stateManager;
	}
protected:
	StateManager&	m_stateManager; //!< Reference to the state manager.
	bool			m_transparent; //!< If the state can be drawn over.
	bool			m_transcendent; //!< If other states can update over it.
};