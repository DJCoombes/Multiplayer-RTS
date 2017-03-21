#pragma once

/**
* @Author	DJ Coombes
* @date		17th March 2017
* @brief	Used to handle the attacking and firing of weapons.
*/

#include "systemBase.h"

class SystemAttack : public SystemBase {
public:
	SystemAttack(SharedContext* context);

	void Update(EntityContainer& entities, float timeStep);
	void Draw(EntityContainer& entities);

	void HandleEvent(const EntityID& entity, const EntityEvent& event);
	void Notify(const Message& message);
private:
	void CheckBulletDistance(std::shared_ptr<Entity> entity);

	SharedContext* m_sharedContext; //!< Pointer to the shared context.
};