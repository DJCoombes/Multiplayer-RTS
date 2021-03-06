/**
* @Author	DJ Coombes
* @date		19th November 2016
* @brief	Used to handle the movement of entities.
*/

#include "systemMovement.h"

#include <stdlib.h>
#include <time.h>

#include "componentPosition.h"
#include "componentMovement.h"
#include "mathFuncs.h"

SystemMovement::SystemMovement(SharedContext* context) 
	: m_sharedContext(context) {
	srand((unsigned int)time(NULL));
}

void SystemMovement::Update(EntityContainer& entities, float timeStep) {
	for (auto& i : entities) {
		auto mc = i->Get<ComponentMovement>();
		if (mc == nullptr)
			continue;
		auto pc = i->Get<ComponentPosition>();
		if (pc == nullptr)
			continue;

		sf::Vector2f movePos = mc->m_moveTo;

		if (!mc->m_atPos) {
			// Check if the entity is within 3 pixels of its destination.
			if (pc->m_position.x < mc->m_moveTo.x + 3 && pc->m_position.x > mc->m_moveTo.x - 3
				&& pc->m_position.y < mc->m_moveTo.y + 3 && pc->m_position.y > mc->m_moveTo.y - 3 && i->GetType() != "bullet") {
				mc->m_atPos = true;
				mc->m_velocity.x = 0;
				mc->m_velocity.y = 0;
				// Give it a new random position to go to.
				int x = rand() & 1240 + 20;
				int y = rand() & 680 + 20;

				mc->MoveTo(sf::Vector2f((float)x, (float)y));

				continue;
			}

			if (mc->m_velocity.x == 0 && mc->m_velocity.y == 0) {
				// Calculate the velocity needed to get to the position.
				mc->m_velocity = CalcVelocity(pc->m_position, mc->m_moveTo, mc->m_speed, timeStep);
			}
		}
		pc->m_position += mc->m_velocity;
	}
}

void SystemMovement::Draw(EntityContainer& entities) {}

void SystemMovement::HandleEvent(const EntityID& entity, const EntityEvent& event) {}

void SystemMovement::Notify(const Message& message) {}