#include "ParserError.h"
#include <string>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif



ParserError::ParserError(Token token, std::string message) : message(message), token(token) {};

char const* ParserError::what() const {
     std::string var = "Parser error [";
     var += message;
     var += "], Token: ";
     var += std::string(token);
     char* cstr = new char[var.size() + 1];
     strcpy(cstr, var.c_str());
     return  cstr;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif