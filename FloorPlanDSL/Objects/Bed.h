#pragma once
#include "IndoorObject.h"

class Bed : public IndoorObject {
public:
	Bed(std::map<TokenType, Object*> params) : IndoorObject(params) {};
	std::string toString() {
		return "Bed";
	};
};
