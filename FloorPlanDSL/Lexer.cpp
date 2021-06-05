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

void Lexer::seek(int offSet) {
     readPosition = col = offSet;
     ch = input[readPosition];

     position = readPosition;
     readPosition++;
}

void Lexer::readChar() {
     if (readPosition >= input.size()) ch = 0;
     else ch = input[readPosition];

     if (ch == '\n') line++, col=0;
     else col++;

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

// while inf
Token Lexer::readNumber() {
     int start = position;
     std::string literal = "";
     Token tok(line, col);
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

// whiel infinite
std::string Lexer::readHexCode() {
     Token tok;
     int start = position;
     
     if (ch == '#') readChar();
     while (isAlpha(ch)) {
          readChar();
     }

     return input.substr(start, position - start);
     
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
     Token tok(line, col);
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
     case '[':
          tok = Token(ch, LBRACKET, line, col);
          readChar();
          break;
     case ']':
          tok = Token(ch, RBRACKET, line, col);
          readChar();
          break;
     case '!':
          tok = Token(ch, EXCLAM_MARK, line, col);
          readChar();
          break;
     case '"': {
          int endStringPos = input.find('"', position + 1);

          if (endStringPos == std::string::npos)
          {
               tok = Token(ch, ILLEGAL, line, col);
               readChar();
          }
          else {
               // check
               std::string str = input.substr(position + 1, endStringPos - position - 1);
               tok = Token(str, STRING_LITERAL, line, col);
               seek(endStringPos + 1);
          }
          break;
     };
          
     case '#':
          tok = Token(readHexCode(), COLOR_LITERAL, line, col);

          if (tok.literal.size() != 4 && tok.literal.size() != 7) {
               tok.type == ILLEGAL;
          }
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