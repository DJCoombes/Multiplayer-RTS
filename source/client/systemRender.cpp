/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Used to handle the drawing of entities with a graphics component.
*/

#include "systemRender.h"

#include <SFML/Graphics.hpp>

#include "componentGraphics.h"
#include "componentPosition.h"
#include "componentSelect.h"

SystemRender::SystemRender(SharedContext* context) : m_sharedContext(context) {
	m_selectedOutline.setFillColor(sf::Color(0, 0, 0, 0));
	m_selectedOutline.setOutlineColor(sf::Color(0, 0, 0));
	m_selectedOutline.setOutlineThickness(6);
}

void SystemRender::Update(EntityContainer& entities, float timeStep) {}

void SystemRender::Draw(EntityContainer& entities) {
	for (auto& i : entities) {
		auto gc = i->Get<ComponentGraphics>();
		if (gc == nullptr)
			continue;

		auto pc = i->Get<ComponentPosition>();
		if (pc == nullptr)
			continue;

		sf::RectangleShape object;
		object.setSize(pc->m_size);
		object.setOutlineColor(sf::Color::Black);
		object.setFillColor(sf::Color::Red);
		object.setPosition(pc->m_position);

		m_sharedContext->m_window->GetWindow().draw(object);

		auto sc = i->Get<ComponentSelect>();
		if (sc == nullptr || !sc->selected)
			continue;

		m_selectedOutline.setRadius(pc->m_size.x - 20.f);
		m_selectedOutline.setPosition(pc->m_position);

		m_sharedContext->m_window->GetWindow().draw(m_selectedOutline);
	}
}

void SystemRender::HandleEvent(const EntityID& entity, const EntityEvent& event) {}

void SystemRender::Notify(const Message& message) {}