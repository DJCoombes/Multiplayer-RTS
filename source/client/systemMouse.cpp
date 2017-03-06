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

SystemMouse::SystemMouse(SharedContext* context) : m_context(context),
	mousePressed(false) {

}

void SystemMouse::Update(EntityContainer& entities, float timeStep) {
	auto window = &m_context->m_window->GetWindow();

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		mousePressed = false;
	else {
		if (!mousePressed) {
			mousePressed = true;
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
			clickPos = window->mapPixelToCoords(pixelPos);
		}
		// Get the mouses position in the view.
		sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
		mousePos = window->mapPixelToCoords(pixelPos);

		m_selectBox = sf::FloatRect(std::min(clickPos.x, mousePos.x), std::min(clickPos.y, mousePos.y), CalculateWidth(), CalculateHeight());
		if (m_selectBox.width < 1 && m_selectBox.height < 1)
			m_selectBox = sf::FloatRect(std::min(clickPos.x, mousePos.x), std::min(clickPos.y, mousePos.y), 5, 5);
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

		sc->selectBox = sf::FloatRect(x - (sc->selectBox.width / 2), y - (sc->selectBox.height / 2), sc->selectBox.width, sc->selectBox.height);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sc->selectable) {
			if (sc->selectBox.intersects(m_selectBox))
				sc->selected = true;
			else
				sc->selected = false;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && sc->selected) {
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
			sf::Vector2f newPos = window->mapPixelToCoords(pixelPos);

			sf::Packet packet;
			SetPacketType(PacketType::MOVEORDER, packet);
			packet << i->GetID();
			packet << (int)newPos.x << (int)newPos.y;
			m_context->m_client->Send(packet);
		}
	}
}

void SystemMouse::Draw(EntityContainer& entities) {
	m_context->m_window->GetWindow().resetGLStates();
	m_context->m_window->GetWindow().pushGLStates();
	if (mousePressed) {
		sf::VertexArray selectBox(sf::TrianglesFan, 4);
		selectBox[0] = sf::Vertex(sf::Vector2f(clickPos.x, clickPos.y), sf::Color(100, 100, 200, 150));
		selectBox[1] = sf::Vertex(sf::Vector2f(clickPos.x, mousePos.y), sf::Color(100, 100, 200, 150));
		selectBox[2] = sf::Vertex(sf::Vector2f(mousePos.x, mousePos.y), sf::Color(100, 100, 200, 150));
		selectBox[3] = sf::Vertex(sf::Vector2f(mousePos.x, clickPos.y), sf::Color(100, 100, 200, 150));
	
		m_context->m_window->GetWindow().draw(selectBox);
	}
	m_context->m_window->GetWindow().popGLStates();
}

void SystemMouse::HandleEvent(const EntityID& entity, const EntityEvent& event) {}

void SystemMouse::Notify(const Message& message) {}

float SystemMouse::CalculateWidth() {
	if (clickPos.x < mousePos.x)
		return mousePos.x - clickPos.x;
	else return clickPos.x - mousePos.x;
}

float SystemMouse::CalculateHeight() {
	if (clickPos.y < mousePos.y)
		return mousePos.y - clickPos.y;
	else return clickPos.y - mousePos.y;
}
