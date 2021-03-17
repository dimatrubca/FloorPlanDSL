#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>  
#include <map>
#include <utility>

#include "Utils.h"
#include "Lexer.h"
#include "LexerError.h"


char Lexer::peekChar() {
     if (readPosition < input.size()) return input[readPosition];
     else return 0;
}

void Lexer::readChar() {
     if (readPosition >= input.size()) ch = 0;
     else ch = input[readPosition];

     position = readPosition;
     readPosition++;
}

std::string Lexer::readIdentifier() {
     int start = position;

     while (isAlpha(ch)) {
          readChar();
     }
     return input.substr(start, position - start);
}

Token Lexer::readNumber() {
     int start = position;
     std::string literal = "";
     Token tok;
     tok.type = INT_LITERAL;

     while (isDigit(ch)) {
          readChar();
     }

     if (ch == '.') {
          readChar();

          while (isDigit(ch)) {
               readChar();
          }
          tok.type = FLOAT_LITERAL;
     }

     tok.literal = input.substr(start, position - start);

     return tok;
}

void Lexer::skipWhitespace() {
     while (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') readChar();

     if (ch == '/' && peekChar() == '/') {
          while (ch != '\n') readChar();
          skipWhitespace();
     }
}

/* */

void Lexer::SetInput(std::string& input) {
     this->input = input;
     readPosition = 0;
     readChar();
}


Token Lexer::NextToken() {
     Token tok;
     skipWhitespace();

     switch (ch) {
     case '=':
          tok = Token(ch, ASSIGN, line, col);
          readChar();
          break;
     case '+':
          tok = Token(ch, PLUS, line, col);
          readChar();
          break;
     case '-':
          tok = Token(ch, MINUS, line, col);
          readChar();
          break;
     case '/':
          tok = Token(ch, SLASH, line, col);
          readChar();
          break;
     case '*':
          tok = Token(ch, ASTERISK, line, col);
          readChar();
          break;
     case ',':
          tok = Token(ch, COMMA, line, col);
          readChar();
          break;
     case ';':
          tok = Token(ch, SEMICOLON, line, col);
          readChar();
          break;
     case ':':
          tok = Token(ch, COLON, line, col);
          readChar();
          break;
     case '(':
          tok = Token(ch, LPAREN, line, col);
          readChar();
          break;
     case ')':
          tok = Token(ch, RPAREN, line, col);
          readChar();
          break;
     case '{':
          tok = Token(ch, LBRACE, line, col);
          readChar();
          break;
     case '}':
          tok = Token(ch, RBRACE, line, col);
          readChar();
          break;
     case '"':
          tok = Token(ch, STRING_LITERAL, line, col);
          readChar();
          break;
     case '#':
          tok = Token(ch, COLOR_LITERAL, line, col);
          readChar();
          break;
     case 0:
          tok = Token("", END, line, col);
          readChar();
          break;
     default:
          if (isLetter(ch)) {
               tok.literal = readIdentifier();
               tok.type = LookUpIdent(tok.literal);
          }
          else if (isDigit(ch)) {
               tok = readNumber();
          }
          else {
               tok = Token(ch, ILLEGAL, line, col);
               readChar();
          }
          break;
     }
     return tok;
}