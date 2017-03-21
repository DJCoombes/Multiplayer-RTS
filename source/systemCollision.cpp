/**
* @Author	DJ Coombes
* @date		14th March 2017
* @brief	Used to handle the collision detection between entities.
*/

#include "systemCollision.h"

SystemCollision::SystemCollision(SharedContext* context) : m_sharedContext(context) {
	m_bounds = sf::FloatRect(0.f, 0.f, 1280, 720);
	m_quadTree = QuadTree(0, m_bounds);
}

void SystemCollision::Update(EntityContainer& entities, float timeStep) {
	m_quadTree.Clear();

	for (auto& i : entities) {
		auto cc = i->Get<ComponentCollision>();
		auto pc = i->Get<ComponentPosition>();

		if (cc == nullptr || pc == nullptr)
			continue;

		float x = pc->m_position.x;
		float y = pc->m_position.y;

		sf::FloatRect bounds = cc->m_bounds;

		sf::FloatRect temp(x - (bounds.width / 2), y - (bounds.height / 2), bounds.width, bounds.height);
		cc->m_bounds = temp;

		m_quadTree.Insert(i, cc->m_bounds);
	}
#ifdef SERVER
	std::list<std::shared_ptr<Entity>> collisionList;
	for (auto& i : entities) {
		auto cc = i->Get<ComponentCollision>();
		auto pc = i->Get<ComponentPosition>();

		if (cc == nullptr || pc == nullptr)
			continue;
		collisionList.clear();
		m_quadTree.Retrieve(collisionList, cc->m_bounds);

		for (auto& j : collisionList) {
			auto cc2 = j->Get<ComponentCollision>();
			if (i == j)
				continue;

			if (i->GetType() == "Projectile" && j->GetType() == "Projectile")
				continue;

			if (cc->m_bounds.intersects(cc2->m_bounds)) {
				cc->m_colliding = true;
				i->CallFunction<ComponentCollision>("collide", i->GetID(), j->GetID());
			}
			else {
				cc->m_colliding = false;
			}
		}
	}
#endif
}

void SystemCollision::Draw(EntityContainer& entities) {
#if(defined GAME && defined _DEBUG)
	m_sharedContext->m_window->GetWindow().resetGLStates();
	m_sharedContext->m_window->GetWindow().pushGLStates();

	sf::RectangleShape collisionBox;
	collisionBox.setOutlineThickness(1.0f);
	collisionBox.setFillColor(sf::Color::Transparent);

	for (auto& i : entities) {
		auto cc = i->Get<ComponentCollision>();
		if (cc == nullptr)
			continue;

		collisionBox.setPosition(cc->m_bounds.left, cc->m_bounds.top);
		collisionBox.setSize(sf::Vector2f(cc->m_bounds.width, cc->m_bounds.height));

		if (cc->m_colliding)
			collisionBox.setOutlineColor(sf::Color::Red);
		else
			collisionBox.setOutlineColor(sf::Color::Green);

		m_sharedContext->m_window->GetWindow().draw(collisionBox);
	}

	m_quadTree.Draw(&m_sharedContext->m_window->GetWindow());

	m_sharedContext->m_window->GetWindow().popGLStates();
#endif
}

void SystemCollision::HandleEvent(const EntityID& entity, const EntityEvent& event) {}

void SystemCollision::Notify(const Message& message) {}