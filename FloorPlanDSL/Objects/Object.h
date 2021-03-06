#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <map>

#include "../Parsing/Error.h"
#include "../Utils.h"
#include "../Parsing/Token.h"


#define hexToInt(ch) (ch >= 'A' ? ch - 'A' + 10 : ch - '0')
class Renderer;
class Room;

typedef std::string ObjectType;

const ObjectType INTEGER_OBJ = "INTEGER";
const ObjectType FLOAT_OBJ = "FLOAT";
const ObjectType STRING_OBJ = "STRING";
const ObjectType MEASURE_OBJ = "MEASURE";
const ObjectType COLOR_OBJ = "COLOR";
const ObjectType ARRAY_OBJ = "ARRAY";
const ObjectType ROOM_OBJ = "ROOM";
const ObjectType WALL_OBJ = "WALL";
const ObjectType WINDOW_OBJ = "WINDOW";
const ObjectType DOOR_OBJ = "DOOR";
const ObjectType Bed_OBJ = "BED";
const ObjectType NULL_OBJ = "NULL";
const ObjectType ERROR_OBJ = "ERROR_OBJ";
const ObjectType BUILTIN_OBJ = "BUILTIN";



class Object {
public:
	Object(ObjectType type) : type(type) {};
	ObjectType getType() { return type; };
	virtual std::string toString() = 0;

private:
	ObjectType type;
};

class Builtin : public Object {
public:
	Builtin(Object* (*fn)(std::vector<Object*>&)) : Object(BUILTIN_OBJ), function(fn) { }
	~Builtin() {}
	std::string Inspect() { return "builtin function"; }
	std::string toString() { return ""; }

	Object* (*function)(std::vector<Object*>&);
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
	Color(std::string code) : Object(COLOR_OBJ), hexCode(code) {
		rgb[0] = (hexToInt(code[1]) * 16 + (hexToInt(code[2])));
		rgb[1] = (hexToInt(code[3]) * 16 + (hexToInt(code[4])));
		rgb[2] = (hexToInt(code[5]) * 16 + (hexToInt(code[6])));
	};
	std::string toString() { return hexCode; };

	std::string hexCode;
	int rgb[3];
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

	bool operator==(const Position& other) const { return (abs(this->x - other.x) <= 0.01 && abs(this->y - other.y) <= 0.01); }
	bool operator!=(const Position& other) const { return !(*this == other); }

	std::string toString() { return "{" + std::to_string(x) + ", " + std::to_string(y) + "}"; };
};


// abstract class
class DrawableObject : public Object {
public:
	std::string id;
	std::vector<float> vertices;
	std::vector<float> colors;
	DrawableObject(ObjectType type, std::map<TokenType, Object*> params);

	void draw(Renderer& renderer);

	void init() {
	}

protected:
	static Position getNextPos(Position pos, float length, float alpha);
	static Position getAdjacentPos(Position pos, float width, float alpha);
	static Position getSegmentIntersection(Position a, Position b, Position c, Position d);
	static void addBufferVertices(Position pos1, Position pos2, Position pos3, std::vector<float> &vertices);
	static void addBufferColors(int rgb[3], int count, std::vector<float>& colors);
private:
	unsigned int VAO;
	unsigned int VBO[2];
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