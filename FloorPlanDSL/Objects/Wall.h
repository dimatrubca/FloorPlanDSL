#pragma once
#include "Object.h"

class Wall : public DrawableObject {
public:
	Wall(std::map<TokenType, Object*> params);
	std::string toString() { return "Wall"; };
	GLuint vertexBuffer;

	// properties
	Position start;
	Position end;
	Border border;
	float angle;

	void setVertices();
};
