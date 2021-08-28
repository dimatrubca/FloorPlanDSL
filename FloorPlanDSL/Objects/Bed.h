#pragma once
#include "IndoorObject.h"

class Bed : public IndoorObject {
public:
	Bed(std::map<TokenType, Object*> params, std::vector<Room*> rooms) : IndoorObject(params, rooms) {};
	std::string toString() {
		return "Bed";
	};
};
