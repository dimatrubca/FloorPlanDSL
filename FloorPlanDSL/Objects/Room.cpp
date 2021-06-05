#include "Room.h"


Room::Room(std::map<TokenType, Object*> params) : DrawableObject(ROOM_OBJ, params) {

	if (hasKey(params, SIZE_PROP)) {
		Array* sizeArray = dynamic_cast<Array*>(params[SIZE_PROP]);
		Error::Assert(sizeArray != nullptr, "Invalid sizeArray property value");

		for (auto elem : sizeArray->elements) {
			Measure* length = dynamic_cast<Measure*>(elem);
			Error::Assert(length != nullptr, "Invalid length in sizeArray");

			sizes.push_back(length->value);
		}
	}

	if (hasKey(params, ANGLES_PROP)) {
		Array* anglesArray = dynamic_cast<Array*>(params[ANGLES_PROP]);
		Error::Assert(anglesArray != nullptr, "Invalid angleArray property value");

		for (auto elem : anglesArray->elements) {
			FloatObject* angle = dynamic_cast<FloatObject*>(elem);
			Error::Assert(angle != nullptr, "Invalid angle type in angleArray");

			angles.push_back(angle->value);
		}

		Error::Assert(angles.size() == sizes.size(), "Length of sizeArray and anglesArray must be equal");
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

		startPosition = Position(x, y);
	}

	setVertices();
};