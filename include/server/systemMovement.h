#pragma once

/**
* @Author	DJ Coombes
* @date		19th November 2016
* @brief	Used to handle the movement of entities.
*/

#include "systemBase.h"

class SystemMovement : public SystemBase {
public:
	SystemMovement(SharedContext* context);

	void Update(EntityContainer& entities, float timeStep);
	void Draw(EntityContainer& entities);

	void HandleEvent(const EntityID& entity, const EntityEvent& event);
	void Notify(const Message& message);
private:
	SharedContext* m_sharedContext;
};