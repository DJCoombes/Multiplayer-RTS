#pragma once

/**
* @Author	DJ Coombes
* @date		19th November 2016
* @brief	Used to handle the movement of entities.
*/

#include "systemBase.h"

class SystemMovement : public SystemBase {
public:
	/*!
	  \brief Constructor.
	  \param context Pointer to the shared context.
	*/
	SystemMovement(SharedContext* context);

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
private:
	SharedContext* m_sharedContext; //!< Pointer to the shared context.
};