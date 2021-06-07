#pragma once
#include "IndoorObject.h"

class Table : public IndoorObject {
public:
	Table(std::map<TokenType, Object*> params) : IndoorObject(params) {};
	std::string toString() {
		return "Table";
	};
};
