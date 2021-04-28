#include "Object.h"
#include "Error.h"
#include "Renderer.h"
#include <math.h>
#define _USE_MATH_DEFINES

#include <cmath>
#include <corecrt_math_defines.h>

Position DrawableObject::getNextPos(Position pos, float length, float alpha) {
	Position nextPos;

	nextPos.x = pos.x + cos(alpha) * length;
	nextPos.y = pos.y + sin(alpha) * length;

	return nextPos;
}

Position DrawableObject::getAdjacentPos(Position pos, float width, float alpha) {
	Position adjacentPos;

	float beta = M_PI / 2 - alpha;

	adjacentPos.x = pos.x - cos(beta) * width;
	adjacentPos.y = pos.y + sin(beta) * width;

	return adjacentPos;
}

void DrawableObject::addBufferVertices(Position pos1, Position pos2, Position pos3, std::vector<float> &vertices) {
	vertices.push_back(pos1.x);
	vertices.push_back(pos1.y);
	vertices.push_back(0);

	vertices.push_back(pos2.x);
	vertices.push_back(pos2.y);
	vertices.push_back(0);

	vertices.push_back(pos3.x);
	vertices.push_back(pos3.y);
	vertices.push_back(0);
}


void DrawableObject::addBufferColors(int rgb[3], int count, std::vector<float> &colors) {
	for (int i = 0; i < count; i++) colors.insert(colors.end(), rgb, rgb + 3);
};

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


std::string Room::toString() {
	std::stringstream ss;
	
	ss << "Room(\n";
	ss << "id = " << id << ",\n";
	ss << "size = {";

	bool first = true;
	for (auto length : sizes) {
		if (!first) ss << ", ";

		ss << length;
		first = false;
	}

	ss << "},\n";

	ss << "angles = {";

	first = true;
	for (auto angle : angles) {
		if (!first) ss << ", ";

		ss << angle;
		first = false;
	}
	ss << "}, \n";

	ss << "border: " << border->toString() << '\n';
	ss << "position: " << startPosition.toString() << '\n';

	return ss.str();
}

Position::Position(Measure* x, Measure* y) {
	this->x = x->value;
	this->y = y->value;
}

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

void Room::setVertices() {
	Position currPos = startPosition;
	float prevAngle = 0;

	float width = this->border->width->value;

	std::cout << M_PI;

	for (int i = 0; i < sizes.size(); i++) {
		float angle = angles[i]/180*M_PI + prevAngle - M_PI * (i != 0); // ???

		Position nextPos = getNextPos(currPos, sizes[i], angle);
		Position currAdjacent = getAdjacentPos(currPos, width, angle);
		Position nextAdjacent = getAdjacentPos(nextPos, width, angle);

		addBufferVertices(currPos, nextPos, currAdjacent, vertices);
		addBufferVertices(nextPos, currAdjacent, nextAdjacent, vertices);

		currPos = nextPos;
		prevAngle = angle;
	}

	addBufferColors(border->color->rgb, sizes.size() * 6, this->colors); // double check
	init();
}

DrawableObject::DrawableObject(TokenType token, std::map<TokenType, Object*> params) : Object(token) {
	if (hasKey(params, ID_PROP)) {
		String* idOb = dynamic_cast<String*>(params[ID_PROP]);
		this->id = idOb->value;
	}
};




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

GLenum glCheckError_2(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		exit(0);
	}
	return errorCode;
}
#define glCheckError2() glCheckError_2(__FILE__, __LINE__)

void DrawableObject::draw(Renderer& renderer) {
	glCheckError2();
	this->vertices;
	renderer.draw(VAO, this->vertices.size() / 3);
	glCheckError2();
}
