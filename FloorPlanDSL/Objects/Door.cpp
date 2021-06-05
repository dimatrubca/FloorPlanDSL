#pragma once
#include "Door.h"

Door::Door(std::map<TokenType, Object*> params, std::vector<Room*> rooms) : DrawableObject(DOOR_OBJ, params) {
	// TODO: require props
	/*String* idParent = dynamic_cast<Door*>(params[ID_PARENT_PROP]);
	Integer* wallNr = dynamic_cast<String*>(params[WALL_PROP]);
	Measure* startOnWall = dynamic_cast<Measure*>(params[START_ON_WALL_PROP]);
	Measure* length = dynamic_cast<Measure*>(params[LENGTH_PROP]);

	if (!idParent || !wallNr || !startOnWall || !length) {
		std::cout << "Invalid Door props";
		// throw error
	}

	Room* parentRoom = nullptr;

	for (auto room : rooms)
	{
		if (room->id == idParent->value) {
			parentRoom = room;
			break;
		}
	}

	if (!parentRoom) {
		std::cout << "Invalid parent id";
		// throw
	}

	if (parentRoom->vertices.size() < wallNr->value) {
		std::cout << "Invalid Wall Number";
	}

	// set start pos
	// set end pos

	//Position start(parentRoom->vertices.);

	///parentRoom->vertices[0];*/
}