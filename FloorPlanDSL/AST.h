#pragma once
#include "Token.h"
#include<vector>
#include<map>
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
	CallExpression,
	Identifier,
	IntLiteral,
	FloatLiteral,
	MeasureLiteral,
	ColorLiteral,
	ArrayLiteral,
	StringLiteral
};

extern std::map<AstNodeType, std::string> AstNodeTypeToString;

// declare interfaces as abstract classes with pure virtual functions
class Node {
public:
	Node(Token token) : token(token) {};
	std::string tokenLiteral() { return token.literal; }

	virtual std::string toString() = 0;
	virtual AstNodeType getType() = 0;

	Token token;
};


class StatementNode: public Node {
public:
	StatementNode(Token token) : Node(token) {};
	virtual std::string toString() = 0;
	virtual AstNodeType getType() = 0;
};


class ExpressionNode: public Node {
public:
	ExpressionNode(Token token) : Node(token) {};
	virtual std::string toString() = 0;
	virtual AstNodeType getType() = 0;
};


// Root Node
class ProgramNode : public Node {
public:
	ProgramNode(Token token) : Node(token) {};
	std::string toString();
	AstNodeType getType() { return AstNodeType::Program; }

	std::vector<StatementNode*> statements;
};


// expressions
class IdentifierNode : public ExpressionNode {
public:
	IdentifierNode(Token token, std::string value) : ExpressionNode(token), value(value) {}
	std::string toString() { return value; }
	AstNodeType getType() { return AstNodeType::Identifier; }

	std::string value;
};


class AssignmentStatementNode : public StatementNode {
public:
	AssignmentStatementNode(Token token) : StatementNode(token) {};
	std::string toString();
	AstNodeType getType() { return AstNodeType::AssignmentStatement; };

	IdentifierNode* varName;
	ExpressionNode* value;
};

// <declarationStatement>: dataType <Identifier> = <Expression> 
class DeclarationStatementNode : public StatementNode {
public:
	DeclarationStatementNode(Token token) : StatementNode(token) {};
	std::string toString();
	AstNodeType getType() { return AstNodeType::DeclarationStatement; }

	TokenType dataType;
	IdentifierNode* varName;
	ExpressionNode* value;
};

class ExpressionStatementNode : public StatementNode {
public:
	ExpressionStatementNode(Token token) : StatementNode(token) {};
	std::string toString();
	AstNodeType getType() { return AstNodeType::ExpressionStatement; }

	ExpressionNode* expression;
};

class PropertyNode : public ExpressionNode {
public:
	PropertyNode(Token token, TokenType name) : ExpressionNode(token), name(name) {};
	std::string toString();
	AstNodeType getType() { return AstNodeType::Property; }

	TokenType name;
	ExpressionNode* value;
};

class StructureStatementNode : public StatementNode {
public:
	StructureStatementNode(Token token, TokenType structureType) : StatementNode(token), structureType(structureType) {};
	std::string toString();
	AstNodeType getType() { return AstNodeType::Structure; }

	TokenType structureType;
	std::vector<PropertyNode*> properties;
};

// e.g: 1, 3, 133, -13???
class IntegerLiteralNode : public ExpressionNode {
public:
	IntegerLiteralNode(Token token, int value) : ExpressionNode(token), value(value) {}
	std::string toString() { return std::to_string(value); }
	AstNodeType getType() { return AstNodeType::IntLiteral; }

	int value;
};

// e.g: 3.14, 1.0, -1???
class FloatLiteralNode : public ExpressionNode {
public:
	FloatLiteralNode(Token token, double value) : ExpressionNode(token), value(value) {}
	std::string toString() { return std::to_string(value); }
	AstNodeType getType() { return AstNodeType::FloatLiteral; }

	double value;
};


// e.g: 3cm, 2cm, (2+3)dm, 3*(1+3)cm
class MeasureLiteralNode : public ExpressionNode {
public:
	MeasureLiteralNode(Token token, ExpressionNode* valueExpr, TokenType unit) : ExpressionNode(token), valueExpr(valueExpr), unit(unit) {}
	std::string toString() { return valueExpr->toString() + unit; };
	AstNodeType getType() { return AstNodeType::FloatLiteral; }

	ExpressionNode* valueExpr;
	TokenType unit;
};


// e.g: #FFF938
class ColorLiteralNode : public ExpressionNode {
public:
	ColorLiteralNode(Token token, std::string value) : ExpressionNode(token), value(value) {}
	std::string toString() { return token.literal; }
	AstNodeType getType() { return AstNodeType::ColorLiteral; }

	std::string value;
};


// e.g: "mystring", "this is a string"
class StringLiteralNode : public ExpressionNode {
public:
	StringLiteralNode(Token token, std::string value) : ExpressionNode(token), value(value) {}
	std::string toString() { return "\"" + value + "\""; }
	AstNodeType getType() { return AstNodeType::StringLiteral; }

	std::string value; // without quotes
};

// e.g: {1, 3, 5}, {}
class ArrayLiteralNode : public ExpressionNode {
public:
	ArrayLiteralNode(Token token) : ExpressionNode(token) {};
	std::string toString();
	AstNodeType getType() { return AstNodeType::ArrayLiteral; }

	std::vector<ExpressionNode*> elements;
};

// e.g: 1+4, a+4
class InfixExpressionNode : public ExpressionNode {
public:
	InfixExpressionNode(Token token, TokenType op, ExpressionNode* left) : ExpressionNode(token), op(op), left(left) {};
	std::string toString();
	AstNodeType getType() {
		return AstNodeType::InfixExpression;
	}

	TokenType op;
	ExpressionNode* left;
	ExpressionNode* right;
};

// <prefix operator><expression>
// e.g: -3
class PrefixExpressionNode : public ExpressionNode {
public:
	PrefixExpressionNode(Token token, TokenType op) : ExpressionNode(token), op(op) {};
	std::string toString();
	AstNodeType getType() {
		return AstNodeType::PrefixExpression;
	}

	TokenType op;
	ExpressionNode* right;
};

// e.g: a[3], a[b] 
class IndexExpressionNode : public ExpressionNode {
public:
	IndexExpressionNode(Token token, ExpressionNode* left) : ExpressionNode(token), left(left) {};
	std::string toString();
	AstNodeType getType() {
		return AstNodeType::IndexExpression;
	}

	ExpressionNode* left;
	ExpressionNode* index;
};

class CallExpressionNode : public ExpressionNode {
public:
	CallExpressionNode(Token token, ExpressionNode* function) : ExpressionNode(token), function(function) {};
	std::string toString();
	AstNodeType getType() {
		return AstNodeType::CallExpression;
	}

	ExpressionNode* function;
	std::vector<ExpressionNode*> arguments;
};

