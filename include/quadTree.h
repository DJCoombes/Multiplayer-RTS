#pragma once

/**
* @Author	DJ Coombes
* @date		14th March 2017
* @brief	Splits entities up into areas, easier to search for collisions.
*/

#include <array>
#include <list>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "entity.h"

class QuadTree {
public:
	/*!
	  \brief Constructor.
	*/
	QuadTree();

	/*!
	  \brief Constructor.
	  \param level Level of the quad tree.
	  \param bounds An SFML rectangle of the bounds.
	*/
	QuadTree(int level, sf::FloatRect bounds);

	/*!
	  \brief Clear everything out of the quad tree.
	*/
	void Clear();

	/*!
	  \brief Split into a new quad tree.
	*/
	void Split();

	/*!
	  \brief Get the index that an entity belongs to.
	  \return int with the position of the quad tree.
	*/
	int GetIndex(sf::FloatRect rect);

	/*!
	  \brief Insert an entity into the quad tree.
	  \param entity Pointer to the entity to insert onto the quad tree.
	  \param rect Rectangle of the entities collision box.
	*/
	void Insert(std::shared_ptr<Entity> entity, sf::FloatRect rect);

	/*!
	  \brief Retrieve a list of entities that are in the same collision box.
	*/
	void Retrieve(std::list<std::shared_ptr<Entity>>& entitys, sf::FloatRect rect);

#if(defined GAME && defined _DEBUG)
	/*!
	  \brief Draw the quad tree.
	*/
	void Draw(sf::RenderWindow* window);
#endif
private:
	int m_level; //!< Level of the quad tree.
	sf::FloatRect m_bounds; //!< Bounds of the quad tree.
	std::array<std::shared_ptr<QuadTree>, 4> m_nodes; //!< Quad tree nodes.
	std::vector<std::shared_ptr<Entity>> m_entities; //!< Entities with collision boxes.
	std::vector<sf::FloatRect> m_collisionBoxes; //!< Collision box of the entities.
	int m_maxObjects; //!< Maximum number of entities in a section before it subdivides.
	int m_maxLevels; //!< Maximum levels of a quad tree.
#if(defined GAME && defined _DEBUG)
	sf::RectangleShape m_drawBox;
#endif
};