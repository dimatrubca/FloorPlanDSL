#pragma once
#include <iostream>


class LexerError : public std::exception
{
public:
     LexerError(char const* message, int line, int col);
     virtual char const* what() const;

private:
     char const* message;
     int line, col;
};