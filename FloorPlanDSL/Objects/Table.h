#pragma once
#include "IndoorObject.h"

class Table : public IndoorObject {
public:
	Table(std::map<TokenType, Object*> params, std::vector<Room*> rooms) : IndoorObject(params, rooms) {};
	std::string toString() {
		return "Table";
	};
};
