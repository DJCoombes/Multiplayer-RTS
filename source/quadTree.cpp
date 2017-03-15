/**
* @Author	DJ Coombes
* @date		14th March 2017
* @brief	Splits entities up into areas, easier to search for collisions.
*/

#include "quadTree.h"

QuadTree::QuadTree() {

}

QuadTree::QuadTree(int level, sf::FloatRect bounds) : m_maxLevels(5), m_maxObjects(20),
m_level(level), m_bounds(bounds) {
	m_nodes[0] = nullptr;
	m_nodes[1] = nullptr;
	m_nodes[2] = nullptr;
	m_nodes[3] = nullptr;
#if(defined GAME && defined _DEBUG)
	m_drawBox.setPosition(m_bounds.left, m_bounds.top);
	m_drawBox.setSize(sf::Vector2f(m_bounds.width, m_bounds.height));
	m_drawBox.setOutlineThickness(10.0f);
	m_drawBox.setOutlineColor(sf::Color::Red);
	m_drawBox.setFillColor(sf::Color::Transparent);
#endif
}

void QuadTree::Clear() {
	m_entities.clear();
	m_collisionBoxes.clear();
	for (auto& i : m_nodes) {
		if (i != nullptr) {
			i->Clear();
			i = nullptr;
		}
	}
}

void QuadTree::Split() {
	float subWidth = m_bounds.width / 2;
	float subHeight = m_bounds.height / 2;
	float x = m_bounds.left;
	float y = m_bounds.top;

	m_nodes[0] = std::make_shared<QuadTree>(m_level + 1, sf::FloatRect(x + subWidth, y, subWidth, subHeight));
	m_nodes[1] = std::make_shared<QuadTree>(m_level + 1, sf::FloatRect(x, y, subWidth, subHeight));
	m_nodes[2] = std::make_shared<QuadTree>(m_level + 1, sf::FloatRect(x, y + subHeight, subWidth, subHeight));
	m_nodes[3] = std::make_shared<QuadTree>(m_level + 1, sf::FloatRect(x + subWidth, y + subHeight, subWidth, subHeight));
}

int QuadTree::GetIndex(sf::FloatRect rect) {
	int index = -1;

	double verticalMid = m_bounds.left + (m_bounds.width / 2);
	double horizontalMid = m_bounds.top + (m_bounds.height / 2);

	bool top = (rect.left < horizontalMid && rect.top + rect.height < horizontalMid);
	bool bottom = (rect.left > horizontalMid);

	if (rect.left < verticalMid && rect.left + rect.width < verticalMid) {
		if (top)
			index = 1;
		else if (bottom)
			index = 2;
	}
	else if (rect.left > verticalMid) {
		if (top)
			index = 0;
		else if (bottom)
			index = 3;
	}
	return index;
}

void QuadTree::Insert(std::shared_ptr<Entity> entity, sf::FloatRect rect) {
	if (m_nodes[0] != nullptr) {
		int index = GetIndex(rect);

		if (index != -1) {
			m_nodes[index]->Insert(entity, rect);
			return;
		}
	}

	m_entities.push_back(entity);
	m_collisionBoxes.push_back(rect);

	if (m_entities.size() > m_maxObjects && m_level < m_maxLevels) {
		if (m_nodes[0] == nullptr)
			Split();

		int i = 0;
		while (i < m_entities.size()) {
			int index = GetIndex(m_collisionBoxes.at(i));
			if (index != -1) {
				m_nodes[index]->Insert(m_entities[i], m_collisionBoxes[i]);
				m_entities.erase(m_entities.begin() + i);
				m_collisionBoxes.erase(m_collisionBoxes.begin() + i);
			}
			else {
				i++;
			}
		}
	}
}

void QuadTree::Retrieve(std::list<std::shared_ptr<Entity>>& entitys, sf::FloatRect rect) {
	int index = GetIndex(rect);
	if (index != -1 && m_nodes[0] != nullptr)
		m_nodes[index]->Retrieve(entitys, rect);

	entitys.insert(entitys.end(), m_entities.begin(), m_entities.end());
}

#if(defined GAME && defined _DEBUG)
void QuadTree::Draw(sf::RenderWindow* window) {
	for (int i = 0; i < 4; i++) {
		if (m_nodes.at(i) != nullptr)
			m_nodes.at(i)->Draw(window);
	}
	window->draw(m_drawBox);
}
#endif