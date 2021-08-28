#pragma once

#include "../../Objects/Wall.h"
#include "GlDrawableObject.h"
#include <glm/vec3.hpp> // glm::vec3

class TriangleStripDrawable : GlDrawableObject {
public:
	TriangleStripDrawable() {};
	void draw(Renderer* renderer, TextRenderer* textRenderer);
protected:
	std::vector<float> vertices;
	glm::vec3 color;
};

