#pragma once
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <map>
#pragma once
#include "Token.h"

class Lexer {
public:
     void SetInput(std::string& input);
     Token NextToken();

private:
     void readChar();
     char peekChar();
     std::string readIdentifier();
     Token readNumber();
     void skipWhitespace();

     std::string input;
     int position;
     int readPosition;
     char ch;
     int line, col;
};