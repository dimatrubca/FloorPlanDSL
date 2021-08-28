#pragma once
#include "IndoorObject.h"

class Chair : public IndoorObject {
public:
	Chair(std::map<TokenType, Object*> params, std::vector<Room*> rooms) : IndoorObject(params, rooms) {};
	std::string toString() {
		return "Chair";
	};
};
