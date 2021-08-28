#pragma once
#include "Window.h"

Window::Window(std::map<TokenType, Object*> params, std::vector<Room*> rooms) : DrawableObject(WINDOW_OBJ, params) {
	// TODO: require props
	String* idParent = dynamic_cast<String*>(params[ID_PARENT_PROP]);
	Integer* wallNr = dynamic_cast<Integer*>(params[WALL_PROP]);
	Measure* startOnWall = dynamic_cast<Measure*>(params[START_ON_WAL_PROP]);
	Measure* length = dynamic_cast<Measure*>(params[LENGTH_PROP]);

	if (!idParent || !wallNr || !startOnWall || !length) {
		std::cout << "Invalid Window props";
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

	if (parentRoom->vertices.size() + 1 < wallNr->value) {
		std::cout << "Invalid Wall Number";
	}

	Position wallStart = parentRoom->vertices[wallNr->value];
	Position wallEnd = parentRoom->vertices[wallNr->value + 1];


	float dX = wallEnd.x - wallStart.x;
	float dY = wallEnd.y - wallStart.y;

	auto angle = atan2(dY, dX);

	this->start = Position(wallStart.x + startOnWall->value * cos(angle), wallStart.y + startOnWall->value * sin(angle));
	this->end = Position(wallStart.x + (startOnWall->value + length->value) * cos(angle), wallStart.y + (startOnWall->value + length->value) * sin(angle));
	this->width = parentRoom->border->width->value;
}