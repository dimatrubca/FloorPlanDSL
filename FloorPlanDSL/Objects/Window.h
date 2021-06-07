#pragma once
#include "Object.h"
#include "Room.h"

class Window : public DrawableObject {
public:
	Window(std::map<TokenType, Object*> params, std::vector<Room*> rooms);
	std::string toString() { return "Window"; };

	//
	int wall;
	Position start, end;
	float width;
};