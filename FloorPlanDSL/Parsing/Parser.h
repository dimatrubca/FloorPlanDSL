#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include "AST.h"
#include "Lexer.h"
#include "../Utils.h"
#include "ParserError.h"

enum class Precendence {
	LOWEST,
	EQUALS,
	SUM,
	PRODUCT,
	PREFIX,
	INDEX,
	CALL
};

class Parser {
public:
	Parser(Lexer& lexer);

	void advance();
	ProgramNode* parseProgram();
	std::vector<ParserError*> getErrors();
private:
	Lexer lexer;
	Token currToken;
	Token peekToken;
	std::map < TokenType, std::function<ExpressionNode* (Parser& const)>> prefixFuncs;
	std::map < TokenType, std::function<ExpressionNode* (Parser& const, ExpressionNode*)>> infixFuncs;
	std::map <TokenType, Precendence> precendences;
	std::vector<ParserError*> errors;



	StatementNode* parseDeclarationStatement();
	AssignmentStatementNode* parseAssignmentStatement();
	StructureStatementNode* parseStructureDeclaration();
	HeaderStatementNode* parseHeader();
	ExpressionStatementNode* parseExpressionStatement();
	PropertyNode* parsePropertyNode();
	ExpressionNode* parseExpression(Precendence precendence, bool);

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
	ExpressionNode* parseCallExpression(ExpressionNode* left);
	ExpressionNode* parseIndexExpression(ExpressionNode* left);
	std::vector<ExpressionNode*> parseExpressionList(TokenType end);

	Precendence currentPrecendence();
	Precendence peekPrecendence();
	void error(Token token, std::string c);
	bool expectPeek(TokenType token);
	void requirePeekPropertyOf(TokenType structureType);
};