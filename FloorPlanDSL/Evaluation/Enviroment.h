#pragma once
#include <vector>

#include "../Objects/Object.h"
#include "../Utils.h"
#include "../Objects/Wall.h"
#include "../Objects/Door.h"
#include "../Objects/Window.h"

class Enviroment {
public:
	Enviroment();
	void set(std::string name, Object* object);
	void update(std::string name, Object* object);
	Object* get(std::string name);
	bool exists(std::string name);
	std::vector<DrawableObject*> getDrawableObjects();
	std::vector<Wall*> getAllWalls();

	int windowWidth = 600;
	int windowHeight = 600;
private:
	std::map<std::string, Object*> store;
	std::vector<Room*> rooms;
	std::vector<Wall*> walls;
	std::vector<Door*> doors;
	std::vector<Window*> windows;
};

