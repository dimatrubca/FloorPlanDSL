#pragma once
#include "TriangleStripDrawable.h"
#include "../../Objects/Room.h"

class DrawableRoom : public TriangleStripDrawable {
public:
	DrawableRoom(Room* room);
	virtual void draw(Renderer* renderer, TextRenderer* textRenderer);
private:
	float area;
	Position center;
	Position findCenter();
	float calculateArea();
	Room* room;
};

