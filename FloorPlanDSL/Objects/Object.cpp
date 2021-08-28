#include "Object.h"
#include "Room.h"
#include "../Rendering/Renderer.h"

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

	adjacentPos.x = pos.x + cos(beta) * width;
	adjacentPos.y = pos.y - sin(beta) * width;

	return adjacentPos;
}

Position DrawableObject::getSegmentIntersection(Position a, Position b, Position c, Position d) {
	double A1 = b.y - a.y;
	double B1 = a.x - b.x;
	double C1 = A1 * b.x + B1 * b.y;

	double A2 = d.y - c.y;
	double B2 = c.x - d.x;
	double C2 = A2 * d.x + B2 * d.y;
	

	// compute intersection
	double det = A1 * B2 - A2 * B1;

	double x = (B2 * C1 - B1 * C2) / det;
	double y = (A1 * C2 - A2 * C1) / det;

	Position pos(x, y);
	return pos;
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


void Room::setVertices() {
	//Position currPos = startPosition;
	//float prevAngle = 0;

	//float width = this->border->width->value;

	//std::vector<Position> points;
	//std::vector<Position> borderPoints;
	//std::vector<Position> adjBorderPoints;


	//for (int i = 0; i < sizes.size(); i++) {
	//	float angle = angles[i] / 180 * M_PI + prevAngle - M_PI * (i != 0); // ?
	//	
	//	Position nextPos = getNextPos(currPos, sizes[i], angle);
	//	Position currAdjacent = getAdjacentPos(currPos, width, angle);
	//	Position nextAdjacent = getAdjacentPos(nextPos, width, angle);
	//	/*
	//	addBufferVertices(currPos, nextPos, currAdjacent, vertices);
	//	addBufferVertices(nextPos, currAdjacent, nextAdjacent, vertices);*/



	//	points.push_back(currPos);
	//	points.push_back(nextPos);

	//	borderPoints.push_back(currAdjacent);
	//	borderPoints.push_back(nextAdjacent);

	//	currPos = nextPos;
	//	prevAngle = angle;
	//}

	//Position prevIntersection = borderPoints[0];

	//if (abs(points.back().x - points[0].x) <= 0.001 && abs(points.back().y - points[0].x <= 0.001)) {
	//	points.push_back(points[0]);
	//	points.push_back(points[1]);

	//	borderPoints.push_back(borderPoints[0]);
	//	borderPoints.push_back(borderPoints[1]);
	//}
	//
	//for (int i = 0; i + 3 < borderPoints.size(); i += 2) {
	//	Position intersection = getSegmentIntersection(borderPoints[i], borderPoints[i + 1],
	//		borderPoints[i + 2], borderPoints[i + 3]);


	//	adjBorderPoints.push_back(prevIntersection);
	//	adjBorderPoints.push_back(intersection);
	//	prevIntersection = intersection;
	//}

	//adjBorderPoints.push_back(adjBorderPoints.back());
	//adjBorderPoints.push_back(borderPoints.back());

	//for (int i = 0; i < points.size() && i < adjBorderPoints.size(); i += 2) {
	//	addBufferVertices(points[i], points[i+1], adjBorderPoints[i], vertices);
	//	addBufferVertices(points[i + 1], adjBorderPoints[i], adjBorderPoints[i + 1], vertices);
	//}

	//addBufferColors(border->color->rgb, sizes.size() * 6, this->colors); // double check
	//init();
}

DrawableObject::DrawableObject(TokenType token, std::map<TokenType, Object*> params) : Object(token) {
	if (hasKey(params, ID_PROP)) {
		String* idOb = dynamic_cast<String*>(params[ID_PROP]);
		this->id = idOb->value;
	}
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
	renderer.draw(VAO, this->vertices.size() / 3);
	glCheckError2();
}
