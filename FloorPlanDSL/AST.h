#pragma once
#include "Token.h"
#include<vector>
#include<iostream>


enum class AstNodeType {
	Program,
	AssignmentStatement,
	DeclarationStatement,
	ExpressionStatement,
	Structure,
	Property,
	PrefixExpression,
	InfixExpression,
	IndexExpression,
	Identifier,
	IntLiteral,
	FloatLiteral,
	MeasureLiteral,
	ColorLiteral,
	ArrayLiteral,
	StringLiteral
};

// declare interfaces as abstract classes with pure virtual functions
class Node {
public:
	virtual std::string toString() = 0;
	virtual AstNodeType getType() = 0;
};


class StatementNode: public Node {
public:
	virtual std::string toString() = 0;
	virtual AstNodeType getType() = 0;
};


class ExpressionNode: public Node {
public:
	virtual std::string toString() = 0;
	virtual AstNodeType getType() = 0;
};


// Root Node
class ProgramNode : public Node {
public:
	ProgramNode(Token token) : token(token) {};
	std::string toString();
	AstNodeType getType() { return AstNodeType::Program; }

	Token token; // first token
	std::vector<StatementNode*> statements;
};

class AssignmentStatementNode : public StatementNode {
public:
	AssignmentStatementNode(Token token) : token(token) {};
	std::string toString();
	AstNodeType getType() { return AstNodeType::AssignmentStatement; }

	Token token;
	IdentifierNode* variable;
	ExpressionNode* value;
};

class DeclarationStatementNode : public StatementNode {
public:
	DeclarationStatementNode(Token token) : token(token) {};
	std::string toString();
	AstNodeType getType() { return AstNodeType::DeclarationStatement; }

	Token token;
	IdentifierNode* varType;
	IdentifierNode* varName;
	ExpressionNode* value;
};

class StructureStatementNode : public StatementNode {
public:
	StructureStatementNode(Token token, TokenType structureType) : token(token), structureType(structureType) {};
	std::string toString();
	AstNodeType getType() { return AstNodeType::Structure; }

	Token token;
	TokenType structureType;
	std::vector<PropertyNode*> properties;
};


class PropertyNode : public ExpressionNode {
public:
	PropertyNode(Token token, TokenType name) : token(token)m value(value) {};
	std::string toString();
	AstNodeType getType() { return AstNodeType::Property; }

	Token token;
	TokenType name;
	ExpressionNode* value;
};

// expressions
class IdentifierNode : public ExpressionNode {
public:
	IdentifierNode(Token token, std::string value): token(token), value(value) {}
	std::string toString() { return value; }
	AstNodeType getType() { return AstNodeType::Identifier; }

	Token token;
	std::string value;
};


class IntegerLiteralNode : public ExpressionNode {
public:
	IntegerLiteralNode(Token token, int value) : token(token), value(value) {}
	std::string toString() { return std::to_string(value); }
	AstNodeType getType() { return AstNodeType::IntLiteral; }

	Token token;
	int value;
};


class FloatLiteralNode : public ExpressionNode {
public:
	FloatLiteralNode(Token token, float value) : token(token), value(value) {}
	std::string toString() { return std::to_string(value); }
	AstNodeType getType() { return AstNodeType::FloatLiteral; }

	Token token;
	float value;
};


class MeasureLiteralNode : public ExpressionNode {
public:
	MeasureLiteralNode(Token token, ExpressionNode* valueExpr, TokenType unit) : token(token), valueExpr(valueExpr), unit(unit) {}
	std::string toString() { return valueExpr->toString() + unit; };
	AstNodeType getType() { return AstNodeType::FloatLiteral; }

	Token token;
	ExpressionNode* valueExpr;
	TokenType unit;
};


class ColorLiteralNode : public ExpressionNode {
public:
	ColorLiteralNode(Token token, std::string value) : token(token), value(value) {}
	std::string toString() { return "#" + token.literal; }
	AstNodeType getType() { return AstNodeType::ColorLiteral; }

	Token token;
	std::string value;
};



class StringLiteralNode : public ExpressionNode {
public:
	StringLiteralNode(Token token, std::string value) : token(token), value(value) {}
	std::string toString() { return value; }
	AstNodeType getType() { return AstNodeType::StringLiteral; }

	Token token;
	std::string value;
};

class ArrayLiteralNode : public ExpressionNode {
public:
	ArrayLiteralNode(Token token) : token(token) {};
	std::string toString();
	AstNodeType getType() { return AstNodeType::ArrayLiteral; }

	Token token;
	std::vector<ExpressionNode*> elements;
};

class InfixExpressionNode : public ExpressionNode {
public:
	InfixExpressionNode(Token token, TokenType op, ExpressionNode* left) : token(token), op(op), left(left) {};
	std::string toString();
	AstNodeType getType() {
		return AstNodeType::InfixExpression;
	}

	Token token;
	TokenType op;
	ExpressionNode* left;
	ExpressionNode* right;
};

class PrefixExpressionNode : public ExpressionNode {
public:
	PrefixExpressionNode(Token token, TokenType op) : token(token), op(op) {};
	std::string toString();
	AstNodeType getType() {
		return AstNodeType::PrefixExpression;
	}

	Token token;
	TokenType op;
	ExpressionNode* right;
};
/*
std::string PrefixExpression::toString() {
	std::string out;

	out += "(" + this->op + this->right->toString() + ")";
}
*/








//<expression>[<expression>]
class IndexExpressionNode : public ExpressionNode {
public:
	IndexExpressionNode(Token token, ExpressionNode* left) : token(token), left(left) {};
	std::string toString();
	AstNodeType getType() {
		return AstNodeType::IndexExpression;
	}
	Token token;
	ExpressionNode* left;
	ExpressionNode* index;
};