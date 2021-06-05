#pragma once
#include "Bed.h"

Bed::Bed(std::map<TokenType, Object*> params) : DrawableObject(Bed_OBJ, params) {
	if (hasKey(params, WIDTH_PROP)) {
		Measure* width = dynamic_cast<Measure*>(params[WIDTH_PROP]);

		width->value;
	}
}