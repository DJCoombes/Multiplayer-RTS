/**
* @Author	DJ Coombes
* @date		21st October 2016
* @brief	Manages all systems, including updating, drawing and dispatching events.
*/

#include "systemManager.h"

#include "sharedContext.h"

#ifdef GAME
#include "systemRender.h"
#include "systemMouse.h"
#elif SERVER
#include "systemMovement.h"
#include "systemAttack.h"
#endif

#include "systemCollision.h"

SystemManager::SystemManager(SharedContext& context) : m_context(&context) {
	// Depending on the instance, create unique instances of the systems.
#ifdef GAME
	m_systems.emplace_back(std::make_unique<SystemRender>(&context));
	m_systems.emplace_back(std::make_unique<SystemMouse>(&context));
#elif SERVER
	m_systems.emplace_back(std::make_unique<SystemMovement>(&context));
	m_systems.emplace_back(std::make_unique<SystemAttack>(&context));
#endif
	m_systems.emplace_back(std::make_unique<SystemCollision>(&context));
}

SystemManager::~SystemManager() {}

void SystemManager::Update(const float& deltaTime) {
#ifdef GAME
	std::lock_guard<std::mutex> lock(m_context->m_client->GetMutex());
#endif
	for (auto& i : m_systems) {
		i->Update(m_context->m_entityManager->GetEntities(), deltaTime);
	}
	HandleEvents();
}
#ifdef GAME
void SystemManager::Draw() {
	for (auto& i : m_systems) {
		i->Draw(m_context->m_entityManager->GetEntities());
	}
}
#endif
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