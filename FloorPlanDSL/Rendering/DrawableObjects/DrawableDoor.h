#pragma once
#include "TriangleStripDrawable.h"
#include "../../Objects/Door.h"

class DrawableDoor : public TriangleStripDrawable {
public:
	DrawableDoor(Door* door);
	virtual void draw(Renderer* renderer, TextRenderer* textRenderer);
private:
	Door* door;
	std::vector<float> lineVertices;
};

