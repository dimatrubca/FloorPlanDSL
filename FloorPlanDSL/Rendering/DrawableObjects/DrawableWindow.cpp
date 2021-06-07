#include "DrawableWindow.h"


DrawableWindow::DrawableWindow(Window* window) : window(window) {
	float dX = window->end.x - window->start.x;
	float dY = window->end.y - window->start.y;

	auto angle = atan2(dY, dX);
	auto length = sqrt(dX * dX + dY * dY);
	Position start2, end2;
	float width = window->width;

	start2.x = window->start.x - width * sin(angle);
	start2.y = window->start.y + width * cos(angle);

	end2.x = window->end.x - width * sin(angle);
	end2.y = window->end.y + width * cos(angle);


	this->vertices.push_back(window->start.x);
	this->vertices.push_back(window->start.y);
	this->vertices.push_back(1);

	this->vertices.push_back(start2.x);
	this->vertices.push_back(start2.y);
	this->vertices.push_back(1);

	this->vertices.push_back(window->end.x);
	this->vertices.push_back(window->end.y);
	this->vertices.push_back(1);

	this->vertices.push_back(end2.x);
	this->vertices.push_back(end2.y);
	this->vertices.push_back(1);

	this->color = glm::vec3(255.0, 255.0, 255.0);


	this->lineVertices.push_back(window->start.x);
	this->lineVertices.push_back(window->start.y);
	this->lineVertices.push_back(3);

	this->lineVertices.push_back(window->end.x);
	this->lineVertices.push_back(window->end.y);
	this->lineVertices.push_back(3);

}

void DrawableWindow::draw(Renderer* renderer) {
	renderer->drawTriangleStrip(vertices, color);
	renderer->drawLineStrip(lineVertices, glm::vec3(157, 193, 255));
}