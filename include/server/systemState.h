#pragma once

/**
* @Author	DJ Coombes
* @date		27th February 2017
* @brief	The state system handles the updating and changing of Lua states for each of the entities.
*/

#include "systemBase.h"

class SystemState : public SystemBase {
public:
	SystemState(SharedContext* context);

	void Update(EntityContainer& entities, float timeStep);
	void Draw(EntityContainer& entities);

	void HandleEvent(const EntityID& entity, const EntityEvent& event);
	void Notify(const Message& message);
private:
	SharedContext* m_sharedContext;
};