#pragma once
#include "Bed.h"

Bed::Bed(std::map<TokenType, Object*> params) : DrawableObject(Bed_OBJ, params) {
	if (hasKey(params, WIDTH_PROP)) {
		Measure* width = dynamic_cast<Measure*>(params[WIDTH_PROP]);

		this->width = width->value;
	}

	if (hasKey(params, HEIGHT_PROP)) {
		Measure* height = dynamic_cast<Measure*>(params[HEIGHT_PROP]);

		this->height = height->value;
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
}