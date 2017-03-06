#pragma once

/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Used to handle the drawing of entities with a graphics component.
*/

#include "systemBase.h"

//#include <SFML/Graphics/CircleShape.hpp>
#include <gl_core_4_4.hpp>

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

	//sf::CircleShape	m_selectedOutline;
	GLfloat* vertices;
	GLuint VAO;
	GLuint VBO;
	Shader m_shader;
};