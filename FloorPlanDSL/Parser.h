#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include "AST.h"
#include "Lexer.h"
#include "Utils.h"

enum class Precendence {
	LOWEST,
	EQUALS,
	SUM,
	PRODUCT,
	PREFIX,
	INDEX
};

class Parser {
public:
	Parser(Lexer& lexer);

	void advance();
	Program* parseProgram();
	std::vector<std::string> getErrors();
private:
	Lexer lexer;
	Token currToken;
	Token peekToken;
	std::vector<std::string> errors;
	std::map < TokenType, std::function<ExpressionNode* (Parser& const)>> prefixFuncs;
	std::map < TokenType, std::function<ExpressionNode* (Parser& const, ExpressionNode*)>> infixFuncs;
	std::map <TokenType, Precendence> precendences;

	StatementNode* parseDeclarationStatement();
	StatementNode* parseAssignmentStatement();
	StructureNode* parseStructureDeclaration();
	ExpressionNode* parseExpression(Precendence precendence);

	ExpressionNode* parseIdentifier();
	ExpressionNode* parseIntegerLiteral();
	ExpressionNode* parseFloatLiteral();
	ExpressionNode* parseStringLiteral();
	ExpressionNode* parseMeasureLiteral();
	ExpressionNode* parseColorLiteral();
	ExpressionNode* parsePrefixExpression();
	ExpressionNode* parseArrayLiteral();
	ExpressionNode* parseGroupedExpression();
	ExpressionNode* parseIndexExpression();
	ExpressionNode* parseInfixExpression(ExpressionNode* left);
	ExpressionNode* parseIndexExpression(ExpressionNode* left);

	Precendence peekPrecendence();
	void addError(std::string c);
	void requireSymbol(char c);
	void requireToken(TokenType token);
};