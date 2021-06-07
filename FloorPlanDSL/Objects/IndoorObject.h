#pragma once
#include "Object.h"

class IndoorObject : public DrawableObject {
public:
	IndoorObject(std::map<TokenType, Object*> params);

	// properties
	float width, height;
	float rotation;
	Position position; // top left corner
};
