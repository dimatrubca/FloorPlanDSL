#include <string>
#include <iostream>
#include <fstream>
#include "Parsing/Lexer.h"
#include "Parsing/Parser.h"
#include "Evaluation/Evaluator.h"
#include <variant>

void printParserErrors(std::vector<ParserError*> const &errors) {
     for (auto err : errors) {
          std::cout << err->what() << "\n";
     }
}

void rlpl() {
     std::ifstream file("res/sample.txt");
     std::string content((std::istreambuf_iterator<char>(file)),
          std::istreambuf_iterator<char>());


     Lexer lexer;
     lexer.SetInput(content);
     Token t = lexer.NextToken();
     while (t.type != END) {
          std::cout << "Type: ' " << t.type << " ', literal: " << t.literal << std::endl;
          t = lexer.NextToken();
     }
}

void rppl() {
     std::ifstream file("res/sample.txt");
     std::string content((std::istreambuf_iterator<char>(file)),
          std::istreambuf_iterator<char>());


     Lexer lexer;
     lexer.SetInput(content);
     Parser parser(lexer);
     auto program = parser.parseProgram();

     if (parser.getErrors().size() != 0) {
          printParserErrors(parser.getErrors());
          return;
     }

     std::cout << "\n\n\n";
     std::cout << program->toString();
}

void ConsoleRPPL() {
     std::string input;

     while (std::getline(std::cin, input)) {
          Lexer lexer;
          lexer.SetInput(input);
          Parser parser(lexer);
          ProgramNode* program = parser.parseProgram();

          if (parser.getErrors().size() != 0) {
               printParserErrors(parser.getErrors());
               continue;
          }

          std::cout << program->toString();
          std::cout << '\n';
     }
}

void ConsoleREPL() {
     std::string input;
     Evaluator evaluator(new Enviroment());

     while (std::getline(std::cin, input)) {
          Lexer lexer;
          lexer.SetInput(input);
          Parser parser(lexer);
          ProgramNode* program = parser.parseProgram();

          if (parser.getErrors().size() != 0) {
               printParserErrors(parser.getErrors());
               continue;
          }

          Object* evalResult = evaluator.eval(program);
          if (evalResult) {
               std::cout << evalResult->toString();
               std::cout << '\n';
          }
     }
}

union RecordType
{
     char ch;
     int i;
     float f;
};

int main2() {
     ConsoleREPL();
     RecordType t;
     t.i = 5;
     std::cout << t.i;
     std::variant<int, float> a, b;
     a = 3.5f;
     b = 3;

     return 0;
}

