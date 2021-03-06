#include "Enviroment.h"

Enviroment::Enviroment() {
	set("black", new Color("#000000"));
	set("silver", new Color("#C0C0C0"));
	set("gray", new Color("#808080"));
	set("white", new Color("#FFFFFF"));
	set("maroon", new Color("#800000"));
	set("red", new Color("#FF0000"));
	set("purple", new Color("#800080"));
	set("fuchsia", new Color("#FF00FF"));
	set("green", new Color("#008000"));
	set("lime", new Color("#00FF00"));
	set("olive", new Color("#808000"));
	set("yellow", new Color("#FFFF00"));
	set("navy", new Color("#000080"));
	set("blue", new Color("#0000FF"));
	set("teal", new Color("#008080"));
	set("aqua", new Color("#00FFFF"));
}

void Enviroment::set(std::string name, Object* object) {
	store[name] = object;

	if (auto wall = dynamic_cast<Wall*>(object)) {
		walls.push_back(wall);
		return;
	}

	if (auto room = dynamic_cast<Room*>(object)) {
		rooms.push_back(room);
		return;
	}

	if (auto door = dynamic_cast<Door*>(object)) {
		doors.push_back(door);
		return;
	}

	if (auto bed = dynamic_cast<Bed*>(object)) {
		beds.push_back(bed);
	}

	if (auto window = dynamic_cast<Window*>(object)) {
		windows.push_back(window);
	}

	if (auto table = dynamic_cast<Table*>(object)) {
		tables.push_back(table);
	}

	if (auto chair = dynamic_cast<Chair*>(object)) {
		chairs.push_back(chair);	
	}
}

void Enviroment::update(std::string name, Object* object) {
	if (!hasKey(store, name)) {
		//throw
		std::cout << "Exception while updateing variable inside enviroment";
		return;
	}

	if (get(name)->getType() != object->getType()) {
		std::cout << "Invalid type of variable to update";
		return;
	}

	// delete existing one
	store[name] = object;
}

bool Enviroment::exists(std::string name) {
	return hasKey(store, name);
}

Object* Enviroment::get(std::string name) {
	if (!hasKey(store, name)) {
		//throw
		std::cout<<"ERROR::Invalid identifier " << name << "\n";
	}
	return store[name];
}

std::vector<DrawableObject*> Enviroment::getDrawableObjects() {
	std::vector<DrawableObject*> drawables;

	for (auto it = store.begin(); it != store.end(); it++)
	{
		if (auto drawableObj = dynamic_cast<DrawableObject*>(it->second))
		{
			drawables.push_back(drawableObj);
		}
	}

	return drawables;
}