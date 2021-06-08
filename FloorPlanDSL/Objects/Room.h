#pragma once
#include "Object.h"

class Room : public DrawableObject {
public:
	Room(std::map<TokenType, Object*> params);
	std::string toString();

	// properties
	std::string label;
	std::vector<float> sizes;
	std::vector<float> angles;
	std::vector<Position> vertices;
	Position startPosition;
	Border* border;

	void setVertices();
};