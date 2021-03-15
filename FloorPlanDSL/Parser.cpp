#include <vector>
#include <string>
#include <iostream>
#include "Lexer.h"
#include "AST.h"

class Parser {


public:
	Parser(std::vector<Token> tokens);

private:
	std::vector<Token> tokens;
	Token currToken;
	Token peekToken;

	Program* parseProgram();
	void advance();
	void error(std::string c);
	void requireSymbol(char c);
	void requireKeyword();
	Statement* parseDeclarationStatement();
	Statement* parseAssignmentStatement();
	Structure* parseStructureDeclaration();
	Expression* parseInfixExpression();
	Identifier* parseIdentifier();
	IntegerLiteral* parseIntegerLiteral();
	Expression* parseFloatLiteral();
	Expression* parseStringLiteral();
	Expression* parseMeasureLiteral();
	Expression* parseColorLiteral();

};


Program* Parser::parseProgram() {
	Program* program = new Program();
	Statement* statement;
	advance();

	while (currToken.type != END) {
		if (isDataType(currToken)) {
			statement = parseDeclarationStatement();
		} else if (isStructureName(currToken)) {
			statement = parseStructureDeclaration();
		} else if (currToken.type == IDENTIFIER) {
			statement = parseAssignmentStatement();
		} else {
			
		}

		program->statements.push_back(statement);
	}

	return program;
}

Statement* Parser::parseDeclarationStatement() {
	DeclarationStatement* statement = new DeclarationStatement();

	statement->varType = parseIdentifier();
	statement->varName = parseIdentifier();
	statement->value = parseInfixExpression();

	return statement;
}

IntegerLiteral* Parser::parseIntegerLiteral() {
	IntegerLiteral* integerLiteral = new IntegerLiteral();

	integerLiteral->token = currToken;
	integerLiteral->value = std::stoi(currToken.literal);

	return integerLiteral;
}

Structure* Parser::parseStructureDeclaration() {
	Structure* structure = new Structure();

	// require structure identifier!!!
	if (structures.find(currToken.type) == structures.end()) {
		// raise Exception
	}

	structure->name = parseIdentifier();

	do {
		PropertyNode* prop = new PropertyNode();
		prop->name = parseIdentifier();
		prop->value = parseInfixExpression();

		structure->properties.push_back(prop);
	} while (currToken.type == COMMA);

	return nullptr;
}




/*
void Parser::requireSymbol(char symbol) {
	advance();
	if (tokenizer.tokenType() == TokenType::Symbol && tokenizer.symbol() == symbol) {
		// print
	}
	else {
		error("'" + std::string(1, symbol) + "'");
	}
}*/