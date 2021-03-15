#include <string>
#include <iostream>
#include <fstream>
#include "Lexer.h"


int main() {
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