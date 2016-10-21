#pragma once

#include <memory>

#include "entity.h"
#include "observer.h"
#include "eventQueue.h"
#include "entityEvents.h"
#include "sharedContext.h"

using EntityContainer = std::vector<std::shared_ptr<Entity>>; //!< Container for the entities.

class SystemBase : public Observer {
public:
	/*!
	  \brief Constructor.
	  \param context Shared pointer to the shared context.
	*/
	SystemBase(std::shared_ptr<SharedContext> context) 
		: m_sharedContext(context) {}

	/*!
	  \brief Update this system, must be implemented in derived classes.
	  \param entities Entities to update.
	  \param timeStep Time since last update.
	*/
	virtual void Update(EntityContainer& entities, float timeStep) = 0;

	/*!
	  \brief Handle all the queued event.
	  \param entity ID of the entity.
	  \param event Event that's happened.
	*/
	virtual void HandleEvent(const EntityID& entity, const EntityEvent& event) = 0;

	/*!
	  \brief Draw everything relating to this system.
	  \param entities Vector of shared pointer to entities.
	*/
	virtual void Draw(EntityContainer& entities) = 0;
private:
	std::shared_ptr<SharedContext> m_sharedContext; //!< Shared pointer to the shared context.
};