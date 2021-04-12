#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <map>

#include "Utils.h"

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
	Border(Measure* width, Color* color) : width(width), color(color) {};
	Measure* width;
	Color* color;

	std::string toString() { return "{" + width->toString() + ", " + color->toString() + "}"; };
};

struct Position {
	Position(Measure* x, Measure* y) : x(x), y(y) {};
	Measure* x;
	Measure* y;

	std::string toString() { return "{" + x->toString() + ", " + y->toString() + "}"; };
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

class Wall : public Object {
public:
	Wall(std::map<TokenType, Object*> params);
	std::string toString() {return "Wall"; };

	// properties
	Position start;
	Position end;
	Border border;
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