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
	InfixExpression,
	Identifier,
	IntLiteral,
	FloatLiteral,
	MeasureLiteral,
	ColorLiteral,
	ArrayLiteral,
	StringLiteral
};

// declare interfaces as abstract classes
class Node {
public:
	virtual std::string toString() = 0;
	virtual AstNodeType getType() = 0;
};


class Statement: public Node {
public:
	virtual std::string toString() = 0;
	virtual AstNodeType getType() = 0;
};


class Expression: public Node {
public:
	virtual std::string toString() = 0;
	virtual AstNodeType getType() = 0;
};


// root node
class Program : public Node {
public:
	std::string toString();
	AstNodeType getType() { return AstNodeType::Program; }


	std::vector<Statement*> statements;
};

// expressions

class Identifier : Expression {
public:
	std::string toString() { return value; }
	AstNodeType getType() { return AstNodeType::Identifier; }

	Token token;
	std::string value;
};


class IntegerLiteral : Expression {
public:
	std::string toString() { return std::to_string(value); }
	AstNodeType getType() { return AstNodeType::IntLiteral; }

	Token token;
	int value;
};


class FloatLiteral : Expression {
public:
	std::string toString() { return std::to_string(value); }
	AstNodeType getType() { return AstNodeType::FloatLiteral; }

	Token token;
	float value;
};


class MeasureLiteral : Expression {
public:
	std::string toString() { return valueToken.literal + unitToken.literal; };
	AstNodeType getType() { return AstNodeType::FloatLiteral; }

	Token valueToken;
	Token unitToken;
};


class ColorLiteral : Expression {
public:
	std::string toString() { return "#" + token.literal; };
	AstNodeType getType() { return AstNodeType::ColorLiteral; }

	Token token;
};



class StringLiteral : Expression {
public:
	std::string toString() { return value; }
	AstNodeType getType() { return AstNodeType::IntLiteral; }

	Token token;
	std::string value;
};

class ArrayLiteral : Expression {
public:
	std::string toString();
	AstNodeType getType() { return AstNodeType::ArrayLiteral; }

	std::vector<Expression> elements;
};

class InfixExpression : Expression {
public:
	std::string toString();
	AstNodeType getType() {
		return AstNodeType::InfixExpression;
	}

	Token opToken;
	Expression* left;
	Expression* right;
};


class AssignmentStatement : Statement {
public:
	std::string toString();
	AstNodeType getType() { return AstNodeType::AssignmentStatement; }

	Identifier* Name;
	Expression* value;
};

class DeclarationStatement : public Statement {
public:
	std::string toString();
	AstNodeType getType() { return AstNodeType::DeclarationStatement; }

	Identifier* varType;
	Identifier* varName;
	Expression* value;
};


class PropertyNode : Node {
public:
	std::string toString();
	AstNodeType getType() { return AstNodeType::Property; }

	Identifier* name;
	Expression* value;
};


class Structure : public Statement {
public:
	std::string toString();
	AstNodeType getType() { return AstNodeType::Structure; }

	Identifier* name;
	std::vector<PropertyNode*> properties;
};