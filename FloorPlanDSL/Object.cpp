#include "Object.h"
#include "Error.h"

Measure::Measure(float value, TokenType unitType) : Object(MEASURE_OBJ) {
	float convertFactor = 1;

	if (unitType == MEASURE_UNIT_CM) {
		convertFactor = 10;
	}
	else if (unitType == MEASURE_UNIT_DM) {
		convertFactor = 100;
	}
	else if (unitType == MEASURE_UNIT_M) {
		convertFactor = 1000;
	}

	this->value = value * convertFactor;
}


Room::Room(std::map<TokenType, Object*> params) : Object(ROOM_OBJ) {
	if (hasKey(params, SIZE_PROP)) {
		Array* sizeArray = dynamic_cast<Array*>(params[SIZE_PROP]);
		Error::Assert(sizeArray != nullptr, "Invalid sizeArray property value");

		for (auto elem : sizeArray->elements) {
			Measure* length = dynamic_cast<Measure*>(elem);
			Error::Assert(length != nullptr, "Invalid length in sizeArray");

			size.push_back(length);
		}
	}

	if (hasKey(params, ANGLES_PROP)) {
		Array* anglesArray = dynamic_cast<Array*>(params[ANGLES_PROP]);
		Error::Assert(anglesArray != nullptr, "Invalid angleArray property value");

		for (auto elem : anglesArray->elements) {
			FloatObject* angle = dynamic_cast<FloatObject*>(elem);
			Error::Assert(angle != nullptr, "Invalid angle type in angleArray");

			angles.push_back(angle);
		}

		Error::Assert(angles.size() == size.size(), "Length of sizeArray and anglesArray must be equal");
	}
	/*else {
		for (int i = 0; i < size.size(); i++) angles.push_back(new FloatObject(""));
	}*/

	if (hasKey(params, BORDER_PROP)) {
		Array* borderArray = dynamic_cast<Array*>(params[BORDER_PROP]);
		Measure* width = dynamic_cast<Measure*>(borderArray->elements[0]);
		Color* color = dynamic_cast<Color*>(borderArray->elements[1]);

		Error::Assert(borderArray != nullptr, "Invalid borderArray"); // ? remove
		Error::Assert(width != nullptr, "First element inside borderArray must be of type 'measure'");
		Error::Assert(color != nullptr, "Second element inside borderArray must be of type 'color'");

		border = new Border(width, color);
	}

	if (hasKey(params, POSITION_PROP)) {
		Array* positionsArray = dynamic_cast<Array*>(params[POSITION_PROP]);
		Measure* x = dynamic_cast<Measure*>(positionsArray->elements[0]);
		Measure* y = dynamic_cast<Measure*>(positionsArray->elements[1]);

		//MyError::Assert(positionsArray != nullptr, "Invalid positionsArray");
		//MyError::Assert(x != nullptr, "First element inside positionsArray must be of type 'measure'");
		//MyError::Assert(y != nullptr, "Second element inside positionsArray must be of type 'measure'");

		position = new Position(x, y);
	}

};


std::string Room::toString() {
	std::stringstream ss;
	
	ss << "Room(\n";
	ss << "id = " << id << ",\n";
	ss << "size = {";

	bool first = true;
	for (auto length : size) {
		if (!first) ss << ", ";

		ss << length->toString();
		first = false;
	}

	ss << "},\n";

	ss << "angles = {";

	first = true;
	for (auto angle : angles) {
		if (!first) ss << ", ";

		ss << angle->toString();
		first = false;
	}
	ss << "}, \n";

	ss << "border: " << border->toString() << '\n';
	ss << "position: " << position->toString() << '\n';

	return ss.str();
}

Position::Position(Measure* x, Measure* y) {
	this->x = x->value;
	this->y = y->value;
}

void Wall::setVertices() {
	Position start2, end2;
	float width = this->border.width->value;

	start2.x = start.x - width * sin(angle);
	start2.y = start.y + width * cos(angle);

	end2.x = end.x - width * sin(angle);
	end2.y = end.y + width * cos(angle);

	vertices.push_back(start.x);
	vertices.push_back(start.y);
	vertices.push_back(start2.x);
	vertices.push_back(start2.y);
	vertices.push_back(end2.x);
	vertices.push_back(end2.y);

	vertices.push_back(start.x);
	vertices.push_back(start.y);
	vertices.push_back(end2.x);
	vertices.push_back(end2.y);
	vertices.push_back(end.x);
	vertices.push_back(end.y);
}