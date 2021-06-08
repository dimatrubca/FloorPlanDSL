#pragma once
#include "IndoorObject.h"

#include <cmath>
#include <corecrt_math_defines.h>

IndoorObject::IndoorObject(std::map<TokenType, Object*> params, std::vector<Room*> rooms) : DrawableObject(Bed_OBJ, params) {
	if (hasKey(params, WIDTH_PROP)) {
		Measure* width = dynamic_cast<Measure*>(params[WIDTH_PROP]);

		this->width = width->value;
	}

	if (hasKey(params, HEIGHT_PROP)) {
		Measure* height = dynamic_cast<Measure*>(params[HEIGHT_PROP]);

		this->height = height->value;
	}

	if (hasKey(params, ROTATION_PROP)) {
		Integer* rotation = dynamic_cast<Integer*>(params[ROTATION_PROP]);
		Error::Assert(rotation != nullptr, "Invalid rotation value");
		this->rotation = rotation->value;
	}

	if (hasKey(params, POSITION_PROP)) {
		Array* positionArray = dynamic_cast<Array*>(params[POSITION_PROP]);
		Error::Assert(positionArray != nullptr, "Invalid position property value");
		Error::Assert(positionArray->elements.size() == 2, "Invalid position arguments count");

		Measure* xPos = dynamic_cast<Measure*>(positionArray->elements[0]);
		Measure* yPos = dynamic_cast<Measure*>(positionArray->elements[1]);

		Error::Assert(xPos != nullptr, "Invalid position argument type");
		Error::Assert(xPos != nullptr, "Invalid position argument type");

		this->position = Position(xPos->value, yPos->value);
	}
	else if (hasKey(params, ID_PARENT_PROP)) {
		String* idParent = dynamic_cast<String*>(params[ID_PARENT_PROP]);
		Measure* startOnWall = dynamic_cast<Measure*>(params[START_ON_WAL_PROP]);
		Measure* distanceWall = dynamic_cast<Measure*>(params[DISTANCE_WALL_PROP]);
		Integer* wallNr = dynamic_cast<Integer*>(params[WALL_PROP]);

		Error::Assert(idParent != nullptr, "Invalid id_parent property value");
		Error::Assert(startOnWall != nullptr, "Invalid start_on_wall property value");
		Error::Assert(distanceWall != nullptr, "Invalid distance_wall property value");
		Error::Assert(wallNr != nullptr, "Invalid wall_nr property value");

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

		float angle = atan2(dY, dX);
		float startOnWallX = wallStart.x + startOnWall->value * cos(angle);
		float startOnWallY = wallStart.y + startOnWall->value * sin(angle);


		this->position = getAdjacentPos(Position(startOnWallX, startOnWallY), distanceWall->value, angle);
			this->rotation = (angle - M_PI / 2) * 180 / M_PI; 
			std::cout << "";
		//this->start = Position();
		//this->end = Position(wallStart.x + (startOnWall->value + length->value) * cos(angle), wallStart.y + (startOnWall->value + length->value) * sin(angle));
		//this->width = parentRoom->border->width->value;
	}
}