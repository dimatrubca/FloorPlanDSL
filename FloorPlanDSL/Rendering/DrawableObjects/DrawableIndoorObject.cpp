#pragma once
#include "DrawableIndoorObject.h"
#include "DrawableUtils.h"
#include <glm/vec3.hpp> // glm::vec3

DrawableIndoorObject::DrawableIndoorObject(IndoorObject* indoorObject) : indoorObject(indoorObject) {
	this->rotate = 180;
	this->color = glm::vec3(255, 255, 255);
	this->position = glm::vec2(indoorObject->position.x, indoorObject->position.y);
	this->size = glm::vec2(indoorObject->width, indoorObject->height);
}