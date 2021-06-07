#pragma once
#include "IndoorObject.h"

class Chair : public IndoorObject {
public:
	Chair(std::map<TokenType, Object*> params) : IndoorObject(params) {};
	std::string toString() {
		return "Chair";
	};
};
