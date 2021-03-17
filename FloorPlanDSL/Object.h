#pragma once
#include <string>
#include <iostream>

typedef std::string ObjectType;

const ObjectType INTEGER_OBJ = "INTEGER";
const ObjectType FLOAT_OBJ = "FLOAT";
const ObjectType STRING_OBJ = "STRING";
const ObjectType MEASURE_OBJ = "MEASURE";
const ObjectType NULL_OBJ = "NULL";

class Object {
	virtual ObjectType getType() = 0;
	virtual void print() = 0;
};

class Integer : public Object {
public:
	ObjectType getType() { return INTEGER_OBJ; };
	void print() { std::cout << value; };

	int value;
};

class MEASURE : public Object {
public:
	ObjectType getType() { return MEASURE_OBJ; }
	void print() { std::cout << value; };

	int value;
	std::string unit;
};