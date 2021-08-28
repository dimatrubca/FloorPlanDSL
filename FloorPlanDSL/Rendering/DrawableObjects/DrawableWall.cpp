#pragma once
#include "DrawableWall.h"
#include <glm/vec3.hpp> // glm::vec3

DrawableWall::DrawableWall(Wall* wall) : wall(wall) {
	float dX = wall->end.x - wall->start.x;
	float dY = wall->end.y - wall->start.y;

	auto angle = atan2(dY, dX);
	Position start2, end2;
	float width = wall->border.width->value;

	start2.x = wall->start.x - width * sin(angle);
	start2.y = wall->start.y + width * cos(angle);

	end2.x = wall->end.x - width * sin(angle);
	end2.y = wall->end.y + width * cos(angle);


	this->vertices.push_back(wall->start.x);
	this->vertices.push_back(wall->start.y);
	this->vertices.push_back(0);

	this->vertices.push_back(start2.x);
	this->vertices.push_back(start2.y);
	this->vertices.push_back(0);
	
	this->vertices.push_back(wall->end.x);
	this->vertices.push_back(wall->end.y);
	this->vertices.push_back(0);

	this->vertices.push_back(end2.x);
	this->vertices.push_back(end2.y);
	this->vertices.push_back(0);

	this->color = glm::vec3(wall->border.color->rgb[0], wall->border.color->rgb[1], wall->border.color->rgb[2]);
}