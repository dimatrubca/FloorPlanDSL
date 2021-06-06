#pragma once
#include "Object.h"

class Room : public DrawableObject {
public:
	Room(std::map<TokenType, Object*> params);
	std::string toString();

	// properties
	std::vector<float> sizes;
	std::vector<float> angles;
	std::vector<Position> vertices;
	//std::vector<Position> vertices;
	Position startPosition;
	//std::vector<Measure*> size;
	//std::vector<FloatObject*> angles;
	Border* border;

	void setVertices();
};