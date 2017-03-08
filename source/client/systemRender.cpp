/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Used to handle the drawing of entities with a graphics component.
*/

#include "systemRender.h"

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "componentGraphics.h"
#include "componentPosition.h"
#include "componentSelect.h"

SystemRender::SystemRender(SharedContext* context) : m_sharedContext(context) {
	m_selectedOutline.setFillColor(sf::Color(0, 0, 0, 0));
	m_selectedOutline.setOutlineColor(sf::Color(0, 0, 200));
	m_selectedOutline.setOutlineThickness(3);

	GLfloat vertices[] = {
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,

		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,

		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f
	};

	gl::GenVertexArrays(1, &VAO);
	gl::GenBuffers(1, &VBO);

	gl::BindVertexArray(VAO);

	gl::BindBuffer(gl::ARRAY_BUFFER, VBO);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(vertices), vertices, gl::STATIC_DRAW);

	gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, 3 * sizeof(GLfloat), (GLvoid*)0);
	gl::EnableVertexAttribArray(0);

	gl::BindVertexArray(0);

	m_shader.CompileShader("./resources/shaders/unit.vert");
	m_shader.CompileShader("./resources/shaders/unit.frag");
	m_shader.Link();
	m_shader.Validate();
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

		m_shader.Use();

		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 model;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
		projection = glm::perspective(45.0f, (GLfloat)1280 / (GLfloat)720, 0.1f, 10000.0f);

		float x = pc->m_position.x;
		float y = pc->m_position.y;
		float z = -100;

		GLint viewport[4];
		GLfloat winX, winY, winZ;

		gl::GetIntegerv(gl::VIEWPORT, viewport);
		winX = ((float)pc->m_position.x - (viewport[2] / 2)) / 14;
		winY = ((float)pc->m_position.y - (viewport[3] / 2)) / 8;
		winZ = 0.1;
		glm::vec4 viewportVec((float)viewport[0], (float)viewport[1], 1, 1);

		glm::vec3 pos = glm::unProject(glm::vec3(winX, -winY, winZ), model, projection, viewportVec);

		model = glm::translate(model, pos);
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));

		m_shader.SetUniform("view", view);
		m_shader.SetUniform("projection", projection);
		m_shader.SetUniform("model", model);

		gl::BindVertexArray(VAO);

		gl::DrawArrays(gl::TRIANGLES, 0, 36);

		gl::UseProgram(0);
		gl::BindVertexArray(0);

		auto sc = i->Get<ComponentSelect>();
		if (sc == nullptr || !sc->selected)
			continue;

		m_sharedContext->m_window->GetWindow().resetGLStates();
		m_sharedContext->m_window->GetWindow().pushGLStates();

		m_selectedOutline.setRadius(pc->m_size.x);
		m_selectedOutline.setOrigin(m_selectedOutline.getRadius(), m_selectedOutline.getRadius() / 2);
		m_selectedOutline.setPosition(pc->m_position.x - 7, pc->m_position.y - 5);
		m_sharedContext->m_window->GetWindow().draw(m_selectedOutline);

		m_sharedContext->m_window->GetWindow().popGLStates();
	}
}

void SystemRender::HandleEvent(const EntityID& entity, const EntityEvent& event) {}

void SystemRender::Notify(const Message& message) {}