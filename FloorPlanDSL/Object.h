#pragma once
#include <string>
#include <iostream>

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
	virtual void print() = 0;

private:
	ObjectType type;
};

class Integer : public Object {
public:
	Integer(int value) : Object(INTEGER_OBJ), value(value) {};
	void print() { std::cout << value; };

	int value;

	Integer* operator+(const Integer& first) const
	{
		return new Integer(value + first.value);
	}

	Integer* operator-(const Integer& first) const
	{
		return new Integer(first.value - value); // TODO: double check order
	}

	Integer* operator*(const Integer& first) const
	{
		return new Integer(first.value * value);
	}
};




class Float : public Object {
public:
	Float(float value) : Object(FLOAT_OBJ), value(value) {};
	void print() { std::cout << value; };

	float value;
};


class Measure : public Object {
public:
	Measure(float valueMm) : Object(MEASURE_OBJ), value(valueMm) {};
	Measure(float value, TokenType unit);
	void print() { std::cout << value; };

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


Measure::Measure(float value, TokenType unitType) : Object(MEASURE_OBJ) {
	float convertFactor = 1;

	if (unitType == MEASURE_UNIT_CM) {
		convertFactor = 10;
	} else if (unitType == MEASURE_UNIT_DM) {
		convertFactor = 100;
	}
	else if (unitType == MEASURE_UNIT_MM) {
		convertFactor = 1000;
	}

	this->value = value * convertFactor;
}

struct Border {
	Border(Measure width, Color color) : width(width), color(color) {};
	Measure width;
	Color color;
};

struct Position {
	Position(Measure x, Measure y) : x(x), y(y) {};
	Measure x;
	Measure y;
};

class Room : public Object {
public:
	Room(std::map<TokenType, Object*> params);

	// properties
	std::string id;
	std::vector<Measure> size;
	std::vector<Float> angles;
	Border* border;
	Position* position;
};

class Wall : public Object {
public:
	Wall(std::map<TokenType, Object*> params);

	// properties
	Position start;
	Position end;
	Border border;
};

class Window : public Object {
public:
	Window(std::map<TokenType, Object*> params);

	//
	int wall;
	Measure startOnWall;
	Measure End;
};

Room::Room(std::map<TokenType, Object*> params) : Object(ROOM_OBJ) {
	if (hasKey(params, SIZE_PROP)) {
		Array& sizeArray = dynamic_cast<Array&>(*params[SIZE_PROP]);

		for (auto elem : sizeArray.elements) {
			Measure& length = dynamic_cast<Measure&>(*elem);
			size.push_back(length);
		}
	} 

	if (hasKey(params, ANGLES_PROP)) {
		Array& anglesArray = dynamic_cast<Array&>(*params[ANGLES_PROP]);

		for (auto elem : anglesArray.elements) {
			Float& angle = dynamic_cast<Float&>(*elem);
			angles.push_back(angle);
		}

		if (angles.size() != size.size()) {
			// throw new exception
		}
	}

	if (hasKey(params, BORDER_PROP)) {
		Array& borderArray = dynamic_cast<Array&>(*params[BORDER_PROP]);
		Measure& width = dynamic_cast<Measure&>(*borderArray.elements[0]);
		Color& color = dynamic_cast<Color&>(*borderArray.elements[1]);

		border = new Border(width, color);
	}

	if (hasKey(params, POSITION_PROP)) {
		Array& positionsArray = dynamic_cast<Array&>(*params[POSITION_PROP]);
		Measure& x = dynamic_cast<Measure&>(*positionsArray.elements[0]);
		Measure& y = dynamic_cast<Measure&>(*positionsArray.elements[1]);

		position = new Position(x, y);
	}

};


class Color : public Object {
	Color(std::string code) : Object(COLOR_OBJ), hexCode(code) {};
	void print() { std::cout << hexCode; };

	std::string hexCode;
};

class Array : public Object {
public:
	Array() : Object(ARRAY_OBJ) {};
	void print() {
		std::cout << "[";
		for (auto elem : elements) {
			elem->print();

			std::cout << ", ";
		}
		std::cout << "]";
	}

	std::vector<Object*> elements;
};