#include "Token.h"

std::unordered_map<std::string, TokenType> keywords({
	{"int", INT},
	{"string", STRING},
	{"measure", MEASURE},
	{"color", COLOR},
	{"float", FLOAT},
	{"list", LIST},
	{"Room", ROOM},
	{"Window", WINDOW},
	{"Wall", WALL},
	{"Door", DOOR},
	{"Elevator", ELEVATOR},
	{"Stairs", STAIRS},
	{"Bed", BED},
	{"Table", TABLE},
	{"Chair", CHAIR},
	{"id", ID_PROP},
	{"id_parent", IDPARENT_PROP},
	{"size", SIZE_PROP},
	{"angles", ANGLES_PROP},
	{"border", BORDER_PROP},
	{"position", POSITION_PROP},
	{"start_on_wall", STARTONWAL_PROP},
	{"end_on_wall", ENDONWALL_PROP},
	{"length", LENGTH_PROP},
	{"direction", DIRECTION_PROP},
	{"start", START_PROPERTY},
	{"end", END_PROP},
	{"width", WIDTH_PROP},
	{"height", HEIGHT_PROP},
	{"distance_wall", DISTANCEWALL_PROP},
	{"layer", LAYER_PROP},
	{"hidden", HIDDEN_PROP_VALUE},
	{"visible", VISIBILITY_PROP_VALUE},
	{"mm", MEASURE_UNIT_MM},
	{"cm", MEASURE_UNIT_CM},
	{"dm", MEASURE_UNIT_DM},
	{"m", MEASURE_UNIT_M},
	{"km", MEASURE_UNIT_KM}
});

std::set<TokenType> dataTypes({
	INT, STRING, MEASURE, COLOR, FLOAT, LIST
});

std::set<TokenType> structures({
	ROOM, WINDOW, WALL, DOOR, ELEVATOR, STAIRS, BED, TABLE, CHAIR
});

std::set<TokenType> roomProps({
	ID_PROP, SIZE_PROP, ANGLES_PROP, BORDER_PROP, POSITION_PROP, STARTONWAL_PROP, LAYER_PROP
});	

std::set<TokenType> measureUnits({
	MEASURE_UNIT_MM, MEASURE_UNIT_CM, MEASURE_UNIT_DM, MEASURE_UNIT_M, MEASURE_UNIT_KM
});


std::set<TokenType> wallProps({

});

TokenType LookUpIdent(std::string ident) {
	if (keywords.find(ident) == keywords.end()) return IDENTIFIER;
	return keywords[ident];
}