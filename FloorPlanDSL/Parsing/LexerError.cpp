#include "LexerError.h"
#include <string>

LexerError::LexerError(char const* message, int line, int col) : message(message), line(line), col(col) {};

char const* LexerError::what() const {
     std::string var = "Lexer error [";
     var += message;
     var += "], line ";
     var += line;
     var += ", col: ";
     var += col;

     return var.c_str();
}