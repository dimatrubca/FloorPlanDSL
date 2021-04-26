#include "Renderer.h"

Renderer::Renderer(Shader& shader) {
	this->shader = shader;
	this->initRenderData();
}

void Renderer::drawWall(Wall wall) {
	unsigned int VBO;

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, wall.vertices.size() * sizeof(float), &wall.vertices[0], GL_STATIC_DRAW);
	
	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::mat4 model = glm::mat4(1.0f);
	this->shader.setMat4("model", model);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
