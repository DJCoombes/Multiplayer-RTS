/**
* @Author	DJ Coombes
* @date		19th November 2016
* @brief	Used to handle the movement of entities.
*/

#include "systemMovement.h"

#include "componentPosition.h"
#include "componentMovement.h"
#include "mathFuncs.h"

SystemMovement::SystemMovement(SharedContext* context) 
	: m_sharedContext(context) {

}

void SystemMovement::Update(EntityContainer& entities, float timeStep) {
	for (auto& i : entities) {
		auto mc = i->Get<ComponentMovement>();
		if (mc == nullptr)
			continue;
		auto pc = i->Get<ComponentPosition>();
		if (pc == nullptr)
			continue;

		sf::Vector2f movePos = mc->moveTo;

		if (!mc->atPos) {
			if (pc->m_position.x < mc->moveTo.x + 3 && pc->m_position.x > mc->moveTo.x - 3
				&& pc->m_position.y < mc->moveTo.y + 3 && pc->m_position.y > mc->moveTo.y - 3 && i->GetType() != "bullet") {
				mc->atPos = true;
				mc->velocity.x = 0;
				mc->velocity.y = 0;
				continue;
			}
		}

		if (mc->velocity.x == 0 && mc->velocity.y == 0) {
			mc->velocity = calcVelocity(pc->m_position, mc->moveTo, mc->speed, timeStep);
		}
		pc->m_position += mc->velocity;
		LOG(INFO) << pc->m_position.x << ":" << pc->m_position.y;
	}
}

void SystemMovement::Draw(EntityContainer& entities) {}

void SystemMovement::HandleEvent(const EntityID& entity, const EntityEvent& event) {}

void SystemMovement::Notify(const Message& message) {}