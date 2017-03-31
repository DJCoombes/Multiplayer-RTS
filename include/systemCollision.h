#pragma once

/**
* @Author	DJ Coombes
* @date		14th March 2017
* @brief	Used to handle the collision detection between entities.
*/

#include "systemBase.h"

#include "quadTree.h"

class SystemCollision : public SystemBase {
public:
	/*!
	  \brief Constructor.
	  \param context Pointer to the shared context.
	*/
	SystemCollision(SharedContext* context);

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
	QuadTree m_quadTree; //!< Quad tree for faster collision detection.
	sf::FloatRect m_bounds; //!< Bounds of the collision area.
};