#pragma once

/**
* @Author	DJ Coombes
* @date		14th March 2017
* @brief	Used to handle the collision detection between entities.
*/

#include "systemBase.h"

#include "quadTree.h"

class SystemCollision : public SystemBase {
public:
	SystemCollision(SharedContext* context);

	void Update(EntityContainer& entities, float timeStep);
	void Draw(EntityContainer& entities);

	void HandleEvent(const EntityID& entity, const EntityEvent& event);
	void Notify(const Message& message);
private:
	SharedContext* m_sharedContext; //!< Pointer to the shared context.
	QuadTree m_quadTree; //!< Quad tree for faster collision detection.
	sf::FloatRect m_bounds; //!< Bounds of the collision area.
};