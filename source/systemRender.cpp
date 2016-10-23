#include "systemRender.h"

SystemRender::SystemRender(SharedContext* context) : m_sharedContext(context) {

}

void SystemRender::Update(EntityContainer& entities, float timeStep) {
	LOG(DEBUG) << "System render updating.";
}

void SystemRender::Draw(EntityContainer& entities) {
	LOG(DEBUG) << "System render drawing.";
}

void SystemRender::HandleEvent(const EntityID& entity, const EntityEvent& event) {}

void SystemRender::Notify(const Message& message) {}