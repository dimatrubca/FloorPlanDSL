#pragma once
#include "Object.h"
#include "Room.h"

class IndoorObject : public DrawableObject {
public:
	IndoorObject(std::map<TokenType, Object*> params, std::vector<Room*> rooms);

	// properties
	float width, height;
	float rotation;
	Position position; // top left corner
};
