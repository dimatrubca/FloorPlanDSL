#pragma once
#include "DrawableIndoorObject.h"
#include "../../Objects/Bed.h"

class DrawableBed : public DrawableIndoorObject {
public:
	DrawableBed(Bed* bed) : DrawableIndoorObject((IndoorObject*)bed) {};
	virtual std::string getSpriteName() { return "bed"; }
private:

};

