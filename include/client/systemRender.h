#pragma once

/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Used to handle the drawing of entities with a graphics component.
*/

#include "systemBase.h"

#include <gl.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "shader.h"

class SystemRender : public SystemBase {
public:
	SystemRender(SharedContext* context);

	void Update(EntityContainer& entities, float timeStep);
	void Draw(EntityContainer& entities);
	void HandleEvent(const EntityID& entity, const EntityEvent& event);
	void Notify(const Message& message);
private:
	SharedContext*	m_sharedContext; //!< Pointer to the shared context.

	sf::CircleShape m_selectedOutline; //!< Circle that represents selected units.

	GLfloat* vertices; //!< Vertex data for the entities.
	GLuint VAO; //!< Vertex array object for the entities.
	GLuint VBO; //!< Vertex buffer object for the entities.
	Shader m_shader; //!< Shader used for rendering the entities.
};