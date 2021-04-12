#pragma once
#include "Object.h"
#include "Utils.h"

class Enviroment {
public:
	Enviroment();
	void set(std::string name, Object* object);
	void update(std::string name, Object* object);
	Object* get(std::string name);
	bool exists(std::string name);

private:
	std::map<std::string, Object*> store;
	//std::map<std::string, Room*> rooms;
};

