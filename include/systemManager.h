#pragma once

/**
* @Author	DJ Coombes
* @date		21st October 2016
* @brief	Manages all systems, including updating, drawing and dispatching events.
*/

#include <memory>

#include "systemBase.h"
#include "eventQueue.h"
#include "messageHandler.h"

using EntityEventContainer = std::unordered_map<EntityID, EventQueue>;

//!< Forward declaration.
struct SharedContext;

class SystemManager {
public:
	/*!
	  \brief Constructor.
	  \param context Shared pointer to the shared context.
	*/
	SystemManager(SharedContext& context);

	/*!
	  \brief Destructor.
	*/
	~SystemManager();

	/*!
	  \brief Update all the systems.
	  \param deltaTime Time since last update.
	*/
	void Update(const float& deltaTime);
#ifdef GAME
	/*!
	  \brief Draw all the systems.
	*/
	void Draw();
#endif
	/*!
	  \brief Get the current message handler.
	  \return Pointer to the message handler.
	*/
	MessageHandler* GetMessageHandler();

	/*!
	  \brief Add an event.
	  \param entity ID of the entity.
	  \param event ID of the event to add.
	*/
	void AddEvent(const EntityID& entity, const EventID& event);

	/*!
	  \brief Handle all queued events.
	*/
	void HandleEvents();
private:
	SharedContext*					m_context; //!< Shared pointer to the shared context.
	EntityEventContainer			m_events; //!< Events.
	MessageHandler					m_messages; //!< The message handler.

	std::vector<std::unique_ptr<SystemBase>> m_systems; //!< Vector of unique pointers to systems.
};