#include "systemRender.h"

SystemRender::SystemRender(SharedContext* context) : m_sharedContext(context) {

}

void SystemRender::Update(EntityContainer& entities, float timeStep) {}

void SystemRender::Draw(EntityContainer& entities) {}

void SystemRender::HandleEvent(const EntityID& entity, const EntityEvent& event) {}

void SystemRender::Notify(const Message& message) {}