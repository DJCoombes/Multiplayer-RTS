/**
* @Author	DJ Coombes
* @date		21st October 2016
* @brief	Manages all systems, including updating, drawing and dispatching events.
*/

#include "systemManager.h"

#include "sharedContext.h"

SystemManager::SystemManager(SharedContext& context) : m_context(&context) {
	
}

SystemManager::~SystemManager() {}

void SystemManager::Update(const float& deltaTime) {
	HandleEvents();
}

void SystemManager::Draw() {

}

MessageHandler* SystemManager::GetMessageHandler() {
	return &m_messages;
}

void SystemManager::AddEvent(const EntityID& entity, const EventID& event) {
	m_events[entity].AddEvent(event);
}

void SystemManager::HandleEvents() {
	for (auto& event : m_events) {
		EventID id = 0;
		while (event.second.ProcessEvents(id)) {
			for (auto& systems : m_systems) {
				systems->HandleEvent(event.first, (EntityEvent)id);
			}
		}
	}
}