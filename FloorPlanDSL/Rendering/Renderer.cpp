#include "Renderer.h"

#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

Renderer::Renderer(Shader& shader, Shader& spriteShader) {
	glCheckError0();

	this->mainShader = shader;
	this->spriteShader = spriteShader;
	this->initRenderData();
	this->mainShader.use();
	glCheckError0();
}
/*
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
*/

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
};

void Renderer::draw(GLuint VAO, int count) {
	this->mainShader.use();
	glCheckError0();

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, count); // double check division
	glCheckError0();
	glBindVertexArray(0);
}
/*
void Renderer::drawWall(Wall* wall) {
	this->shader.use();

	float dX = wall->end.x - wall->start.x;
	float dY = wall->end.y - wall->start.y;

	auto angle = atan2(dY, dX);
	Position start2, end2;
	float width = wall->border.width->value;

	start2.x = wall->start.x - width * sin(angle);
	start2.y = wall->start.y + width * cos(angle);

	end2.x = wall->end.x - width * sin(angle);
	end2.y = wall->end.y + width * cos(angle);

	std::vector<Position> vertices;

	vertices.push_back(wall->start);
	vertices.push_back(start2);
	vertices.push_back(wall->end);
	vertices.push_back(end2);

	glm::vec3 color(wall->border.color->rgb[0], wall->border.color->rgb[1], wall->border.color->rgb[2]);

	drawTriangleStrip(vertices, color);
}*/

void Renderer::drawTriangleStrip(std::vector<float> vertices, glm::vec3 color) {
	normalizeVertices(vertices);
	this->mainShader.use();
	glCheckError0();

	unsigned int VBO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glCheckError0();

	glBindVertexArray(this->VAO);
	glCheckError0();
	glEnableVertexAttribArray(0);
	glCheckError0();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glCheckError0();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->mainShader.setVector3f("u_color", color);
	glCheckError0();

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size()/3);
	glBindVertexArray(0);
	glCheckError0();
}

void Renderer::drawLineStrip(std::vector<float> vertices, glm::vec3 color) {
	normalizeVertices(vertices);
	this->mainShader.use();
	
	unsigned int VBO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glCheckError0();

	glBindVertexArray(this->VAO);
	glCheckError0();
	glEnableVertexAttribArray(0);
	glCheckError0();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glCheckError0();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->mainShader.setVector3f("u_color", color);
	glCheckError0();

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 3);
	glBindVertexArray(0);
	glCheckError0();
}

void Renderer::drawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
	normalizeVertices(position);
	normalizeVertices(size);

	this->spriteShader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	//model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	this->spriteShader.setMat4("model", model);
	this->spriteShader.setVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}


void Renderer::initRenderData() {
	// configure VAO/VBO
	unsigned int VBO;
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	/**/
	glGenVertexArrays(1, &this->VAO);
	/**/

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::setWindowDimensions(int reqWidth, int reqHeight, int& width, int& height) {
	if (reqWidth <= maxWindowWidth && reqHeight <= maxWindowHeight) {
		width = reqWidth;
		height = reqHeight;
		return;
	}

	if (reqHeight * 1.0 / reqWidth * maxWindowWidth) {
		width = maxWindowWidth;
		height = reqHeight * 1.0 / reqWidth * width;
		this->unitsPerPixel = reqWidth / width;
	}
	else {
		height = maxWindowHeight;
		width = reqWidth * 1.0 / reqHeight * height;
		this->unitsPerPixel = reqHeight / height;
	}
}

void Renderer::normalizeVertices(std::vector<float>& vertices) {
	for (int i = 0; i + 2 < vertices.size(); i += 3) {
		vertices[i] = vertices[i] / unitsPerPixel;
		vertices[i + 1] = vertices[i + 1] / unitsPerPixel;
	}
}

void Renderer::normalizeVertices(glm::vec2& position) {
	position.x /= unitsPerPixel;
	position.y /= unitsPerPixel;
}