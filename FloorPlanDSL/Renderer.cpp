#include "Renderer.h"

Renderer::Renderer(Shader& shader) {
	glCheckError0();

	this->shader = shader;
	this->initRenderData();
	this->shader.use();
	glCheckError0();

	//glBindVertexArray(VAO);

	// layout (location = 0) in vec3 position
	/*glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 3, GL_FLOAT, false, 0); ///3!!!
	glVertexAttribBinding(0, 0);*/
}

void Renderer::drawWall(Wall wall) {
	unsigned int VBO;

	glGenVertexArrays(1, &this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, wall.vertexBuffer);
	//glBufferData(GL_ARRAY_BUFFER, wall.vertices.size() * sizeof(float), &wall.vertices[0], GL_STATIC_DRAW);
	
	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::mat4 model = glm::mat4(1.0f);
	this->shader.setMat4("model", model);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}


float vertices[] = {
    -0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
};

void Renderer::draw(GLuint VAO, int count) {
	this->shader.use();
	glCheckError0();

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, count); // double check division
	glCheckError0();
	glBindVertexArray(0);
	
	/*
	this->shader.use();
	unsigned int VBO2;
	glGenBuffers(1, &VBO2);
	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);

	glBindVertexArray(VAO2);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glCheckError0();*/
}

void Renderer::initRenderData() {

}