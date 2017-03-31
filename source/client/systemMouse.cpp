/**
* @Author	DJ Coombes
* @date		20th November 2016
* @brief	Used to handle the mouse selection.
*/

#include "systemMouse.h"

#include <SFML/Graphics.hpp>

#include "componentSelect.h"
#include "componentMovement.h"
#include "componentPosition.h"
#include "componentWeapon.h"

SystemMouse::SystemMouse(SharedContext* context) : m_context(context),
	m_mousePressed(false) {

}

void SystemMouse::Update(EntityContainer& entities, float timeStep) {
	auto window = &m_context->m_window->GetWindow();

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		m_mousePressed = false;
	else {
		if (!m_mousePressed) {
			m_mousePressed = true;
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
			m_clickPos = window->mapPixelToCoords(pixelPos);
		}
		// Get the mouses position in the view.
		sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
		m_mousePos = window->mapPixelToCoords(pixelPos);

		m_selectBox = sf::FloatRect(std::min(m_clickPos.x, m_mousePos.x), std::min(m_clickPos.y, m_mousePos.y), CalculateWidth(), CalculateHeight());
		if (m_selectBox.width < 1 && m_selectBox.height < 1)
			m_selectBox = sf::FloatRect(std::min(m_clickPos.x, m_mousePos.x), std::min(m_clickPos.y, m_mousePos.y), 5, 5);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		m_attack = true;

	int targetID = NULL;

	if (m_attack) {
		sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
		sf::Vector2f attackPos = window->mapPixelToCoords(pixelPos);
		m_attackBox = sf::FloatRect(attackPos.x, attackPos.y, 2, 2);
		targetID = FindTarget(entities);
	}

	for (auto& i : entities) {
		auto sc = i->Get<ComponentSelect>();
		if (sc == nullptr)
			continue;
		auto mc = i->Get<ComponentMovement>();
		if (mc == nullptr)
			continue;
		auto pc = i->Get<ComponentPosition>();
		if (pc == nullptr)
			continue;

		float x = pc->m_position.x;
		float y = pc->m_position.y;

		sc->m_selectBox = sf::FloatRect(x - (sc->m_selectBox.width / 2), y - (sc->m_selectBox.height / 2), sc->m_selectBox.width, sc->m_selectBox.height);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sc->m_selectable) {
			if (sc->m_selectBox.intersects(m_selectBox))
				sc->m_selected = true;
			else
				sc->m_selected = false;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && sc->m_selected) {
			if (targetID == NULL)
				MoveTo(i->GetID());
			else if (targetID != i->GetID()) {
				auto wc = i->Get<ComponentWeapon>();
				if (wc == nullptr)
					continue;
				Attack(targetID, i->GetID());
			}
		}
	}

	m_attack = false;
}

void SystemMouse::Draw(EntityContainer& entities) {
	m_context->m_window->GetWindow().resetGLStates();
	m_context->m_window->GetWindow().pushGLStates();
	if (m_mousePressed) {
		sf::VertexArray selectBox(sf::TrianglesFan, 4);
		selectBox[0] = sf::Vertex(sf::Vector2f(m_clickPos.x, m_clickPos.y), sf::Color(100, 100, 200, 150));
		selectBox[1] = sf::Vertex(sf::Vector2f(m_clickPos.x, m_mousePos.y), sf::Color(100, 100, 200, 150));
		selectBox[2] = sf::Vertex(sf::Vector2f(m_mousePos.x, m_mousePos.y), sf::Color(100, 100, 200, 150));
		selectBox[3] = sf::Vertex(sf::Vector2f(m_mousePos.x, m_clickPos.y), sf::Color(100, 100, 200, 150));
	
		m_context->m_window->GetWindow().draw(selectBox);
	}
	m_context->m_window->GetWindow().popGLStates();
}

void SystemMouse::HandleEvent(const EntityID& entity, const EntityEvent& event) {}

void SystemMouse::Notify(const Message& message) {}

float SystemMouse::CalculateWidth() {
	if (m_clickPos.x < m_mousePos.x)
		return m_mousePos.x - m_clickPos.x;
	else return m_clickPos.x - m_mousePos.x;
}

float SystemMouse::CalculateHeight() {
	if (m_clickPos.y < m_mousePos.y)
		return m_mousePos.y - m_clickPos.y;
	else return m_clickPos.y - m_mousePos.y;
}

int SystemMouse::FindTarget(EntityContainer& entities) {
	for (auto& i : entities) {
		auto cc = i->Get<ComponentCollision>();
		if (cc == nullptr)
			continue;

		if (cc->m_bounds.intersects(m_attackBox))
			return i->GetID();
	}

	return NULL;
}

void SystemMouse::MoveTo(int entityID) {
	auto window = &m_context->m_window->GetWindow();
	
	sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
	sf::Vector2f newPos = window->mapPixelToCoords(pixelPos);

	sf::Packet packet;
	SetPacketType(PacketType::MOVEORDER, packet);
	packet << entityID;
	packet << (int)newPos.x << (int)newPos.y;
	m_context->m_client->Send(packet);
}

void SystemMouse::Attack(int targetID, int entityID) {
	sf::Packet packet;
	SetPacketType(PacketType::ATTACKORDER, packet);
	packet << entityID << targetID;
	m_context->m_client->Send(packet);
}