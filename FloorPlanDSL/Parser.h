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
	ProgramNode* parseProgram();
private:
	Lexer lexer;
	Token currToken;
	Token peekToken;
	std::map < TokenType, std::function<ExpressionNode* (Parser& const)>> prefixFuncs;
	std::map < TokenType, std::function<ExpressionNode* (Parser& const, ExpressionNode*)>> infixFuncs;
	std::map <TokenType, Precendence> precendences;

	StatementNode* parseDeclarationStatement();
	AssignmentStatementNode* parseAssignmentStatement();
	StructureStatementNode* parseStructureDeclaration();
	PropertyNode* parsePropertyNode();
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
	ExpressionNode* parseInfixExpression(ExpressionNode* left);
	ExpressionNode* parseIndexExpression(ExpressionNode* left);

	Precendence currentPrecendence();
	Precendence peekPrecendence();
	void error(Token token, std::string c);
	void requirePeek(TokenType token);
};