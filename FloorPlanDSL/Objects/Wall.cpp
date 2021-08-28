#pragma once
#include "Wall.h"


void Wall::setVertices() {
	float dX = end.x - start.x;
	float dY = end.y - start.y;

	angle = atan2(dY, dX);
	Position start2, end2;
	float width = this->border.width->value;

	start2.x = start.x - width * sin(angle);
	start2.y = start.y + width * cos(angle);

	end2.x = end.x - width * sin(angle);
	end2.y = end.y + width * cos(angle);

	vertices.push_back(start.x);
	vertices.push_back(start.y);
	vertices.push_back(0);

	vertices.push_back(start2.x);
	vertices.push_back(start2.y);
	vertices.push_back(0);

	vertices.push_back(end2.x);
	vertices.push_back(end2.y);
	vertices.push_back(0);

	vertices.push_back(start.x);
	vertices.push_back(start.y);
	vertices.push_back(0);

	vertices.push_back(end2.x);
	vertices.push_back(end2.y);
	vertices.push_back(0);

	vertices.push_back(end.x);
	vertices.push_back(end.y);
	vertices.push_back(0);

	for (int i = 0; i < 6; i++) colors.insert(colors.end(), this->border.color->rgb, this->border.color->rgb + 3);
}




Wall::Wall(std::map<TokenType, Object*> params) : DrawableObject(WALL_OBJ, params) {
	if (hasKey(params, START_PROPERTY)) {
		Array* positionsArray = dynamic_cast<Array*>(params[START_PROPERTY]);
		Measure* x = dynamic_cast<Measure*>(positionsArray->elements[0]);
		Measure* y = dynamic_cast<Measure*>(positionsArray->elements[1]);

		start = Position(x, y);
	}

	if (hasKey(params, END_PROP)) {
		Array* positionsArray = dynamic_cast<Array*>(params[END_PROP]);
		Measure* x = dynamic_cast<Measure*>(positionsArray->elements[0]);
		Measure* y = dynamic_cast<Measure*>(positionsArray->elements[1]);

		end = Position(x, y);
	}



	if (hasKey(params, BORDER_PROP)) {
		Array* borderArray = dynamic_cast<Array*>(params[BORDER_PROP]);
		Measure* width = dynamic_cast<Measure*>(borderArray->elements[0]);
		Color* color = dynamic_cast<Color*>(borderArray->elements[1]);

		Error::Assert(borderArray != nullptr, "Invalid borderArray"); // ? remove
		Error::Assert(width != nullptr, "First element inside borderArray must be of type 'measure'");
		Error::Assert(color != nullptr, "Second element inside borderArray must be of type 'color'");

		border = Border(width, color);
	}

	// check color
	//...

	setVertices();
	init();
};