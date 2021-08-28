#pragma once
#include "DrawableIndoorObject.h"
#include "../../Objects/Chair.h"

class DrawableChair : public DrawableIndoorObject {
public:
	DrawableChair(Chair* chair) : DrawableIndoorObject((IndoorObject*)chair) {};
	virtual std::string getSpriteName() { return "chair"; }
};

