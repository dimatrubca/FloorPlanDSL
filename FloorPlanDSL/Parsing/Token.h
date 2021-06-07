#pragma once
#include <string>
#include <unordered_map>
#include <set>

typedef std::string TokenType;

const TokenType IDENTIFIER = "IDENT";

// literals
const TokenType INT_LITERAL = "INT_LITERAL"; // e.g.: 1, 3, 4
const TokenType FLOAT_LITERAL = "FLOAT_LITERAL"; // e.g. 1.23, 3.4133
const TokenType STRING_LITERAL = "STRING_LITERAL"; 
const TokenType COLOR_LITERAL = "COLOR_LITERAL"; // e.g. #FAF123

// operatos
const TokenType ASSIGN = "=";
const TokenType PLUS = "+";
const TokenType MINUS = "-";
const TokenType ASTERISK = "*";
const TokenType SLASH = "/";

const TokenType COMMA = ",";
const TokenType SEMICOLON = ";";
const TokenType COLON = ":";

const TokenType LPAREN = "(";
const TokenType RPAREN = ")";
const TokenType LBRACE = "{";
const TokenType RBRACE = "}";
const TokenType LBRACKET = "[";
const TokenType RBRACKET = "]";

const TokenType EXCLAM_MARK = "!";

// keywords
const TokenType INT = "INT"; 
const TokenType STRING = "STRING";
const TokenType MEASURE = "MEASURE";
const TokenType COLOR = "COLOR";
const TokenType FLOAT = "FLOAT";
const TokenType LIST = "LIST";

const TokenType ROOM = "ROOM";
const TokenType WINDOW = "WINDOW";
const TokenType WALL = "WALL";
const TokenType DOOR = "DOOR";
const TokenType ELEVATOR = "ELEVATOR";
const TokenType STAIRS = "STAIRS";
const TokenType BED = "BED";
const TokenType TABLE = "TABLE";
const TokenType CHAIR = "CHAIR";

const TokenType ID_PROP = "ID_PROPERT";
const TokenType ID_PARENT_PROP = "ID_PARENT_PROPERTY";
const TokenType WALL_PROP = "WALL_PROPERTY";
const TokenType END_ON_WALL = "END_ON_WALL_PROPERTY";
const TokenType SIZE_PROP = "SIZE_PROPERTY";
const TokenType ANGLES_PROP = "ANGLES_PROPERTY";
const TokenType BORDER_PROP = "BORDER_PROPERTY";
const TokenType POSITION_PROP = "POSITION";
const TokenType START_ON_WAL_PROP = "START_ON_WALL";
const TokenType ENDONWALL_PROP = "END_ON_WALL";
const TokenType LENGTH_PROP = "LENGTH";
const TokenType DIRECTION_PROP = "DIRECTION";
const TokenType START_PROPERTY = "START";
const TokenType END_PROP = "END";
const TokenType WIDTH_PROP = "WIDTH";
const TokenType HEIGHT_PROP = "HEIGHT";
const TokenType DISTANCEWALL_PROP = "DISTANCE_WALL";
const TokenType LAYER_PROP = "LAYER";

const TokenType VISIBILITY_PROP_VALUE = "VISIBILITY";
const TokenType HIDDEN_PROP_VALUE = "HIDDEN";
const TokenType VISIBLE_PROP_VALUE = "VISSIBLE";

const TokenType MEASURE_UNIT_MM = "MEASURE_MM";
const TokenType MEASURE_UNIT_CM = "MEASURE_CM";
const TokenType MEASURE_UNIT_DM = "MEASURE_DM";
const TokenType MEASURE_UNIT_M = "MEASURE_M";
const TokenType MEASURE_UNIT_KM = "MEASURE_KM";

const TokenType ILLEGAL = "ILLEGAL";
const TokenType END = "END";


extern std::unordered_map<std::string, TokenType> keywords;
extern std::set<TokenType> structures;
extern std::set<TokenType> roomProps;
extern std::set<TokenType> dataTypes;
extern std::set<TokenType> measureUnits;


struct Token
{
     std::string literal;
     TokenType type;
     int line;
     int col;

     Token() {}
     Token(int line, int col): line(line), col(col) {}
     Token(const std::string& literal, TokenType type, int line, int col) : literal(literal), type(type), line(line), col(col) {}
     Token(char ch, TokenType type, int line, int col) : literal(std::string(1, ch)), type(type), line(line), col(col) {}

     operator std::string() const { return literal + ", type: " + type + ", line: " + std::to_string(line) + ", col: " + std::to_string(col); }
};

TokenType LookUpIdent(std::string ident);

