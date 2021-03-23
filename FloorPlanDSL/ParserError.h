#pragma once
#include <iostream>
#include "Token.h"

class ParserError : public std::exception
{
public:
     ParserError(Token token, std::string message);
     virtual char const* what() const;

private:
     std::string message;
     Token token;
};