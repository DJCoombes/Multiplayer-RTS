/**
* @Author	DJ Coombes
* @date		17th March 2017
* @brief	Used to handle the attacking and firing of weapons.
*/

#include "systemAttack.h"

#include <glm/glm.hpp>
#include <glm/vec2.hpp>

#include "componentWeapon.h"
#include "componentPosition.h"
#include "componentMovement.h"
#include "componentDamage.h"

SystemAttack::SystemAttack(SharedContext* context) : m_sharedContext(context) {

}

void SystemAttack::Update(EntityContainer& entities, float timeStep) {
	for (auto& i : entities) {
		if (i->GetType() == "Projectile")
			CheckBulletDistance(i);

		auto wc = i->Get<ComponentWeapon>();
		auto pc = i->Get<ComponentPosition>();
		if (wc == nullptr || wc->m_targetID == NULL)
			continue;

		auto enemy = m_sharedContext->m_entityManager->GetEntity(wc->m_targetID);


		if (enemy == nullptr) {
			wc->m_targetID = NULL;
			continue;
		}

		auto enemypc = enemy->Get<ComponentPosition>();

		if (!wc->m_canFire) {
			if (wc->m_lastShot.getElapsedTime() > wc->m_rateOfFire)
				wc->m_canFire = true;
			continue;
		}

		int bulletID = m_sharedContext->m_entityManager->Create(wc->m_weaponType);
		auto bullet = m_sharedContext->m_entityManager->GetEntity(bulletID);

		glm::vec2 attackerPos(pc->m_position.x, pc->m_position.y);
		glm::vec2 enemyPos(enemypc->m_position.x, enemypc->m_position.y);

		glm::vec2 norm = glm::normalize(enemyPos - attackerPos);

		glm::vec2 bulletPos = attackerPos + (norm * 2.f);

		sf::Vector2f pos(bulletPos.x, bulletPos.y);

		auto bpc = bullet->Get<ComponentPosition>();
		// TODO: Do some magic here to get position correct so as not to spawn inside the attacker.
		bpc->m_position = pos;
		// TODO: Rotation as well.

		auto dc = bullet->Get<ComponentDamage>();
		dc->m_startPos = pos;

		auto bmc = bullet->Get<ComponentMovement>();
		pc = enemy->Get<ComponentPosition>();
		bmc->MoveTo(pc->m_position);
		wc->m_lastShot.restart();
		wc->m_canFire = false;
	}
}

void SystemAttack::Draw(EntityContainer& entities) {}

void SystemAttack::HandleEvent(const EntityID& entity, const EntityEvent& event) {}

void SystemAttack::Notify(const Message& message) {}

void SystemAttack::CheckBulletDistance(std::shared_ptr<Entity> entity) {
	auto dc = entity->Get<ComponentDamage>();
	auto pc = entity->Get<ComponentPosition>();

	auto pos1 = dc->m_startPos;
	auto pos2 = pc->m_position;

	float distance = std::sqrt((std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2)));
	if (distance > dc->m_range) {
		m_sharedContext->m_entityManager->Destroy(entity->GetID());
	}
}