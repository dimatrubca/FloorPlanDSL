#pragma once
#include <vector>

#include "../Objects/Object.h"
#include "../Utils.h"
#include "../Objects/Wall.h"
#include "../Objects/Door.h"
#include "../Objects/Room.h"
#include "../Objects/Window.h"
#include "../Objects/Bed.h"
#include "../Objects/Table.h"
#include "../Objects/Chair.h"

class Enviroment {
public:
	Enviroment();
	void set(std::string name, Object* object);
	void update(std::string name, Object* object);
	Object* get(std::string name);
	bool exists(std::string name);
	std::vector<DrawableObject*> getDrawableObjects();

	int windowWidth = 600;
	int windowHeight = 600;

	std::vector<Room*> rooms;
	std::vector<Wall*> walls;
	std::vector<Door*> doors;
	std::vector<Window*> windows;
	std::vector<Bed*> beds;
	std::vector<Table*> tables;
	std::vector<Chair*> chairs;

	
	
private:
	std::map<std::string, Object*> store;

};

