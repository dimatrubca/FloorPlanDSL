#pragma once
#include "DrawableIndoorObject.h"
#include "../../Objects/Table.h"

class DrawableTable : public DrawableIndoorObject {
public:
	DrawableTable(Table* table) : DrawableIndoorObject((IndoorObject*)table) {};
	virtual std::string getSpriteName() { return "table"; }
};

