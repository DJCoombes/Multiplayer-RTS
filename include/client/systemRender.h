#pragma once

#include "systemBase.h"

class SystemRender : public SystemBase {
public:
	SystemRender(SharedContext* context);

	void Update(EntityContainer& entities, float timeStep);
	void Draw(EntityContainer& entities);
	void HandleEvent(const EntityID& entity, const EntityEvent& event);
	void Notify(const Message& message);
private:
	SharedContext* m_sharedContext;
};