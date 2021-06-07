#pragma once
#include "SpriteDrawable.h"
#include "../../Objects/IndoorObject.h"

class DrawableIndoorObject : public SpriteDrawable {
public:
	DrawableIndoorObject(IndoorObject* indoorObject);
	virtual std::string getSpriteName() = 0;
private:
	IndoorObject* indoorObject;
};

