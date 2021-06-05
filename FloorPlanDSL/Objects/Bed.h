#pragma once
#include "Object.h"

class Bed : public DrawableObject {
public:
	Bed(std::map<TokenType, Object*> params);
	std::string toString() {
		return "Bed";
	};

	// properties
	float width, height;
	float rotation;
	Position position; // bottom left corner
};
