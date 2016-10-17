#pragma once

/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	This base component stores common methods and variables for all components.
*/

class ComponentBase {
public:
	/*!
	  \brief Common constructor for all components.
	*/
	ComponentBase() : m_enabled(true) {}

	/*!
	  \brief Virtual destructor, needs to be overriden in child classes.
	*/
	virtual ~ComponentBase() {}

	/*! 
	  \brief Checks if the component is enabled.
	  \return True of false depending on if the component is enabled.
	*/
	bool IsEnabled() {
		return m_enabled;
	}

	/*!
	  \brief Enable the component.
	*/
	void Enable() {
		m_enabled = true;
	}

	/*!
	  \brief Disable the component.
	*/
	void Disable() {
		m_enabled = false;
	}

	/*!
	  \brief Toggle the component on or off.
	*/
	void Toggle() {
		m_enabled = !m_enabled;
	}

private:
	bool m_enabled; //!< True if this component is enabled.
};