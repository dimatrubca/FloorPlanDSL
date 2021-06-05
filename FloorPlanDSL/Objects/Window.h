#pragma once
#include "Object.h"

class Window : public Object {
public:
	Window(std::map<TokenType, Object*> params);
	std::string toString() { return "Window"; };

	//
	int wall;
	Measure startOnWall;
	Measure End;
};