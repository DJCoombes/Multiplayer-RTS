#pragma once

/**
* @Author	DJ Coombes
* @date		14th October 2016
* @brief	This shared context will store any objects that need to be shared between states.
*/

#include "window.h"

struct SharedContext {
	/*!
	  \brief Constructor.
	*/
	SharedContext() : m_window(nullptr) {}

	Window* m_window; //!< Pointer to the context window.
};