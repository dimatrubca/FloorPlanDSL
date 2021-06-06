#pragma once
#include "DrawableBed.h"
#include "DrawableUtils.h"
#include <glm/vec3.hpp> // glm::vec3

DrawableBed::DrawableBed (Bed* bed) : bed(bed) {
	this->rotate = 180;
	this->color = glm::vec3(255, 255, 255);
	this->position = glm::vec2(bed->position.x, bed->position.y);
	this->size = glm::vec2(bed->width, bed->height);
}