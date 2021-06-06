#pragma once
#include "GlDrawableObject.h"

class SpriteDrawable : GlDrawableObject {
public:
	SpriteDrawable() {};
	void draw(Renderer* renderer);
protected:
	glm::vec3 color;
	glm::vec2 position;
	glm::vec2 size;
	float rotate;
};