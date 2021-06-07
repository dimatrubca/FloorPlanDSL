#include "DrawableDoor.h"

#include <cmath>
#include <corecrt_math_defines.h>

DrawableDoor::DrawableDoor(Door* door) : door(door) {
	float dX = door->end.x - door->start.x;
	float dY = door->end.y - door->start.y;

	auto angle = atan2(dY, dX);
	auto length = sqrt(dX * dX + dY * dY);
	Position start2, end2;
	float width = door->width;

	start2.x = door->start.x - width * sin(angle);
	start2.y = door->start.y + width * cos(angle);

	end2.x = door->end.x - width * sin(angle);
	end2.y = door->end.y + width * cos(angle);


	this->vertices.push_back(door->start.x);
	this->vertices.push_back(door->start.y);
	this->vertices.push_back(1);

	this->vertices.push_back(start2.x);
	this->vertices.push_back(start2.y);
	this->vertices.push_back(1);

	this->vertices.push_back(door->end.x);
	this->vertices.push_back(door->end.y);
	this->vertices.push_back(1);

	this->vertices.push_back(end2.x);
	this->vertices.push_back(end2.y);
	this->vertices.push_back(1);

	this->color = glm::vec3(255.0, 255.0, 255.0);


	auto initialAngle = angle + M_PI / 2 + M_PI/8;
	auto lastAngle = angle + M_PI + M_PI/16;
	auto step = (lastAngle - initialAngle) / 15.0;


	for (auto alpha = initialAngle; alpha <= lastAngle; alpha += step) {
		auto x = end2.x + length * cos(alpha);
		auto y = end2.y + length * sin(alpha) ;

		Position currPoint(x, y);

		this->lineVertices.push_back(x);
		this->lineVertices.push_back(y);
		this->lineVertices.push_back(0);
	}


}

void DrawableDoor::draw(Renderer* renderer) {
	renderer->drawTriangleStrip(vertices, color);
	renderer->drawLineStrip(lineVertices, glm::vec3(0.0, 0.0, 0.0));
}