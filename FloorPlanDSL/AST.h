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


// root node
class Program : public Node {
public:
	std::string toString();
	AstNodeType getType() { return AstNodeType::Program; }


	std::vector<StatementNode*> statements;
};

// expressions

class Identifier : public ExpressionNode {
public:
	Identifier(Token token, std::string value): token(token), value(value) {}
	std::string toString() { return value; }
	AstNodeType getType() { return AstNodeType::Identifier; }

	Token token;
	std::string value;
};


class IntegerLiteral : public ExpressionNode {
public:
	IntegerLiteral(Token token, int value) : token(token), value(value) {}
	std::string toString() { return std::to_string(value); }
	AstNodeType getType() { return AstNodeType::IntLiteral; }

	Token token;
	int value;
};


class FloatLiteral : public ExpressionNode {
public:
	FloatLiteral(Token token, float value) : token(token), value(value) {}
	std::string toString() { return std::to_string(value); }
	AstNodeType getType() { return AstNodeType::FloatLiteral; }

	Token token;
	float value;
};


class MeasureLiteral : public ExpressionNode {
public:
	MeasureLiteral(ExpressionNode* valueExpr, ExpressionNode* unit) : valueExpr(valueExpr), unit(unit) {}
	std::string toString() { return valueExpr->toString() + unit->toString(); };
	AstNodeType getType() { return AstNodeType::FloatLiteral; }

	Token token;
	ExpressionNode* valueExpr;
	ExpressionNode* unit;
};


class ColorLiteral : public ExpressionNode {
public:
	ColorLiteral(Token token, std::string value) : token(token), value(value) {}
	std::string toString() { return "#" + token.literal; }
	AstNodeType getType() { return AstNodeType::ColorLiteral; }

	Token token;
	std::string value;
};



class StringLiteral : public ExpressionNode {
public:
	StringLiteral(Token token, std::string value) : token(token), value(value) {}
	std::string toString() { return value; }
	AstNodeType getType() { return AstNodeType::StringLiteral; }

	Token token;
	std::string value;
};

class ArrayLiteral : public ExpressionNode {
public:
	std::string toString();
	AstNodeType getType() { return AstNodeType::ArrayLiteral; }

	std::vector<ExpressionNode*> elements;
};

class InfixExpression : public ExpressionNode {
public:
	std::string toString();
	AstNodeType getType() {
		return AstNodeType::InfixExpression;
	}

	Token opToken;
	ExpressionNode* left;
	ExpressionNode* right;
};

class PrefixExpression : public ExpressionNode {
public:
	std::string toString();
	AstNodeType getType() {
		return AstNodeType::PrefixExpression;
	}

	Token token;
	std::string op;
	ExpressionNode* right;
};
/*
std::string PrefixExpression::toString() {
	std::string out;

	out += "(" + this->op + this->right->toString() + ")";
}
*/

class AssignmentStatement : public StatementNode {
public:
	std::string toString();
	AstNodeType getType() { return AstNodeType::AssignmentStatement; }

	Identifier* Name;
	ExpressionNode* value;
};

class DeclarationStatement : public StatementNode {
public:
	std::string toString();
	AstNodeType getType() { return AstNodeType::DeclarationStatement; }

	Token token;
	ExpressionNode* varType;
	ExpressionNode* varName;
	ExpressionNode* value;
};


class PropertyNode : public ExpressionNode {
public:
	std::string toString();
	AstNodeType getType() { return AstNodeType::Property; }

	Identifier* name;
	ExpressionNode* value;
};


class Structure : public StatementNode {
public:
	std::string toString();
	AstNodeType getType() { return AstNodeType::Structure; }

	Identifier* name;
	std::vector<PropertyNode*> properties;
};