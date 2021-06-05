#pragma once
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <map>

#include "Token.h"

class Lexer {
public:
     void SetInput(std::string& input);
     Token NextToken();
private:
     void readChar();
     void seek(int offset);
     char peekChar();
     std::string readIdentifier();
     std::string readHexCode();
     Token readNumber();
     void skipWhitespace();

     std::string input;
     int position;
     int readPosition;
     char ch;
     int line=0, col=0;
};