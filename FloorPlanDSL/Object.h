#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <map>

#include "Utils.h"
#include <GL/glew.h>

class Renderer;

typedef std::string ObjectType;

const ObjectType INTEGER_OBJ = "INTEGER";
const ObjectType FLOAT_OBJ = "FLOAT";
const ObjectType STRING_OBJ = "STRING";
const ObjectType MEASURE_OBJ = "MEASURE";
const ObjectType COLOR_OBJ = "COLOR";
const ObjectType ARRAY_OBJ = "ARRAY";
const ObjectType ROOM_OBJ = "ROOM";
const ObjectType WALL_OBJ = "WALL";
const ObjectType DOOR_OBJ = "DOOR";
const ObjectType NULL_OBJ = "NULL";
const ObjectType ERROR_OBJ = "ERROR_OBJ";

class Object {
public:
	Object(ObjectType type) : type(type) {};
	ObjectType getType() { return type; };
	virtual std::string toString() = 0;

private:
	ObjectType type;
};

class Integer : public Object {
public:
	Integer(int value) : Object(INTEGER_OBJ), value(value) {};
	std::string toString() { return std::to_string(value); };

	int value;

	Integer* operator+(const Integer& right) const
	{
		return new Integer(value + right.value);
	}

	Integer* operator-(const Integer& right) const
	{
		return new Integer(value - right.value);
	}

	Integer* operator*(const Integer& right) const
	{
		return new Integer(value * right.value);
	}
};




class FloatObject : public Object {
public:
	FloatObject(float value) : Object(FLOAT_OBJ), value(value) {};
	std::string toString() { return std::to_string(value); };

	static FloatObject* intToFloat(Integer* intObj) {
		return new FloatObject(intObj->value);
	}

	float value;
};


class Measure : public Object {
public:
	Measure(float valueMm) : Object(MEASURE_OBJ), value(valueMm) {};
	Measure(float value, TokenType unit);
	std::string toString() { return std::to_string(value) + " mm"; };

	float value; // value in mm

	Measure* operator+(const Measure& first) const
	{
		return new Measure(value + first.value);
	}

	Measure* operator-(const Measure& first) const
	{
		return new Measure(first.value - value); // TODO: double check order
	}
};

class Color : public Object {
public:
	Color(std::string code) : Object(COLOR_OBJ), hexCode(code) {};
	std::string toString() { return hexCode; };

	std::string hexCode;
};


class String : public Object {
public:
	String(std::string value) : Object(STRING_OBJ), value(value) {};
	std::string toString() { return value; };

	std::string value;
};


struct Border {
	// float width, string color
	Border() {};
	Border(Measure* width, Color* color) : width(width), color(color) {};
	Measure* width;
	Color* color;

	std::string toString() { return "{" + width->toString() + ", " + color->toString() + "}"; };
};

struct Position {
	Position() {};
	Position(Measure* x, Measure* y);
	Position(float x, float y) : x(x), y(y) {};
	float x;
	float y;

	std::string toString() { return "{" + std::to_string(x) + ", " + std::to_string(y) + "}"; };
};

// abstract class
class DrawableObject : public Object {
public:
	std::vector<float> vertices;
	DrawableObject(ObjectType type);

	void draw(Renderer& renderer);

	void init() {
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);

		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		std::cout << vertices.size() * sizeof(float) << "... " << '\n';
		std::cout << sizeof(vertices) << '\n';	
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
private:
	unsigned int VAO;
	unsigned int VBO;
};




class Room : public Object {
public:
	Room(std::map<TokenType, Object*> params);
	std::string toString();

	// properties
	std::string id;
	std::vector<Measure*> size;
	std::vector<FloatObject*> angles;
	Border* border;
	Position* position;
};

class Wall : public DrawableObject {
public:
	Wall(std::map<TokenType, Object*> params);
	std::string toString() {return "Wall"; };
	GLuint vertexBuffer;

	// properties
	Position start;
	Position end;
	Border border;
	float angle;

	void setVertices();
};

class Window : public Object {
public:
	Window(std::map<TokenType, Object*> params);
	std::string toString() { return "Window"; };

	//
	int wall;
	Measure startOnWall;
	Measure End;
};


class Array : public Object {
public:
	Array() : Object(ARRAY_OBJ) {};
	std::string toString() {
		std::stringstream ss;
		ss << "[";
		bool first = true;
		for (auto elem : elements) {
			if (!first) ss << ", ";
			ss << elem->toString();
			first = false;
		}
		ss << "]";

		return ss.str();
	}

	std::vector<Object*> elements;
};

class MyError2 : public Object {
public:
	MyError2() : Object(ERROR_OBJ) {};
};