#pragma once
#include "Object.h"
#include "Utils.h"

class Enviroment {
public:
	void set(std::string name, Object* object);
	Object* get(std::string name);
	bool exists(std::string name);

private:
	std::map<std::string, Object*> store;
};


void Enviroment::set(std::string name, Object* object) {
	store.insert({ name, object });
}

bool Enviroment::exists(std::string name) {
	return hasKey(store, name);
}

Object* Enviroment::get(std::string name) {
	return store[name];
}