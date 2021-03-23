#include "ParserError.h"
#include <string>

ParserError::ParserError(Token token, std::string message) : message(message), token(token) {};

char const* ParserError::what() const {
     std::string var = "Parser error [";
     var += message;
     var += "], line ";
     var += token.line;
     var += ", col: ";
     var += token.col;

     return var.c_str();
}