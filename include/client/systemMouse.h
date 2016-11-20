#pragma once

/**
* @Author	DJ Coombes
* @date		20th November 2016
* @brief	Used to handle the mouse selection.
*/

#include <SFML/System/Vector2.hpp>

#include "systemBase.h"

class SystemMouse : public SystemBase {
public:
	SystemMouse(SharedContext* context);

	void Update(EntityContainer& entities, float timeStep);
	void Draw(EntityContainer& entities);
	void HandleEvent(const EntityID& entity, const EntityEvent& event);
	void Notify(const Message& message);

	float CalculateWidth();
	float CalculateHeight();
private:
	SharedContext*	m_context; //!< Pointer to the shared context.

	sf::Vector2f	clickPos; //!< Where the mouse clicked.
	sf::Vector2f	mousePos; //!< Where the mouse currently is.

	bool			mousePressed; //!< True if the mouse button has been clicked. 
	bool			m_mouseClicked;

	sf::FloatRect	m_selectBox; //!< The box that's occupying the mouse space.
};