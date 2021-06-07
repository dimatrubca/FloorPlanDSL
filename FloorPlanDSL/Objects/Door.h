#pragma once
#include "Object.h"
#include "Room.h"

class Door : public DrawableObject {
public:
	Door(std::map<TokenType, Object*> params, std::vector<Room*> room);
	std::string toString() {
		return "Door";
	};

	// properties
	Position start, end;
	float width;
};
