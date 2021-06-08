#pragma once
#include "GlDrawableObject.h"

class SpriteDrawable : GlDrawableObject {
public:
	SpriteDrawable() {};
	virtual void draw(Renderer* renderer, TextRenderer* textRenderer);
	virtual std::string getSpriteName() = 0;
protected:
	glm::vec3 color;
	glm::vec2 position;
	glm::vec2 size;
	float rotate;
};