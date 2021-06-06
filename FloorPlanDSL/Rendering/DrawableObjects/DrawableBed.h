#pragma once
#include "SpriteDrawable.h"
#include "../../Objects/Bed.h"

class DrawableBed : public SpriteDrawable {
public:
	DrawableBed(Bed* bed);
private:
	Bed* bed;
};

