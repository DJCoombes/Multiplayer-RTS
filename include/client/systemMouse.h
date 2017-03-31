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
	/*!
	  \brief Constructor.
	  \param context Pointer to the shared context.
	*/
	SystemMouse(SharedContext* context);

	/*!
	  \brief Update this system, must be implemented in derived classes.
	  \param entities Entities to update.
	  \param timeStep Time since last update.
	*/
	void Update(EntityContainer& entities, float timeStep);

	/*!
	  \brief Draw everything relating to this system.
	  \param entities Vector of shared pointer to entities.
	*/
	void Draw(EntityContainer& entities);

	/*!
	  \brief Handle all the queued event.
	  \param entity ID of the entity.
	  \param event Event that's happened.
	*/
	void HandleEvent(const EntityID& entity, const EntityEvent& event);

	/*!
	  \brief Used for sending messages, needs to be implemented in derived classes.
	  \param message Message to send.
	*/
	void Notify(const Message& message);

	/*!
	  \brief Calculate the width of the mouse area.
	  \return Width of the mouse area.
	*/
	float CalculateWidth();

	/*!
	  \brief Calculate the height of the mouse area.
	  \return Height of the mouse area.
	*/
	float CalculateHeight();

	/*!
	  \brief Find the ID of the entity that's being clicked on.
	  \param entity Vector of all the entities.
	  \return ID of the entities.
	*/
	int FindTarget(EntityContainer& entities);

	/*!
	  \brief Tell the server that an entity wants to move to a certain position.
	  \param entityID ID of the entity.
	*/
	void MoveTo(int entityID);

	/*!
	  \brief Tell the server that an entity wants to attack another entity.
	  \param targetID ID of the entity to attack.
	  \pram entityID ID of the entity attacking.
	*/
	void Attack(int targetID, int entityID);
private:
	SharedContext*	m_context; //!< Pointer to the shared context.

	sf::Vector2f	m_clickPos; //!< Where the mouse clicked.
	sf::Vector2f	m_mousePos; //!< Where the mouse currently is.

	bool			m_mousePressed; //!< True if the mouse button has been pressed. 
	bool			m_mouseClicked; //!< True if the mouse has been clicked.

	sf::FloatRect	m_selectBox; //!< The box that's occupying the mouse space.
	sf::FloatRect	m_attackBox; //!< The box used for detecting which entity to attack.

	bool			m_attack; //!< If user wants to attack.
};