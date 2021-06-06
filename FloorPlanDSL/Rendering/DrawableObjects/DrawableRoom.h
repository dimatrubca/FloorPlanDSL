#pragma once
#include "TriangleStripDrawable.h"
#include "../../Objects/Room.h"

class DrawableRoom : public TriangleStripDrawable {
public:
	DrawableRoom(Room* room);
private:
	Room* room;
};

