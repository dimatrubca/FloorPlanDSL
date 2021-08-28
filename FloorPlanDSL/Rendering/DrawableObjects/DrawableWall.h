#pragma once
#include "TriangleStripDrawable.h"
#include "../../Objects/Wall.h"

class DrawableWall : public TriangleStripDrawable {
public:
	DrawableWall(Wall* wall);
private:
	Wall* wall;
};

