#pragma once
#include "Object.h"
#include "Enviroment.h"
#include "AST.h"

class Evaluator {
public:
	Evaluator(Enviroment* env);
	Object* eval(Node* node);

	Enviroment* env;
private:

	Object* evalProgram(ProgramNode* program);
	Object* evalExpressionStatement(ExpressionStatementNode* stmt);
	Object* evalAssignmentStatement(AssignmentStatementNode* stmt);
	Object* evalDeclarationStatement(DeclarationStatementNode* stmt);
	Object* evalHeaderStatement(HeaderStatementNode* stmt);
	Object* evalStructureStatement(StructureStatementNode* structure);
	std::pair<TokenType, Object*> evalPropertyNode(PropertyNode* prop);
	Object* evalPrefixExpression(TokenType op, Object* right);
	Object* evalMinusPrefixOperatorExpression(Object* right);
	Object* evalInfixExpression(TokenType op, Object* left, Object* right);
	Object* evalIntegerInfixExpression(TokenType op, Integer* left, Integer* right);
	Object* evalFloatInfixExpression(TokenType op, FloatObject* left, FloatObject* right);
	Object* evalStringInfixExpression(TokenType op, String* left, String* right);
	Object* evalMeasureInfixExpression(TokenType op, Measure* left, Measure* right);
	Object* evalIndexExpression(IndexExpressionNode* expr);
	Object* evalIdentifier(IdentifierNode* ident);
	Object* evalIntLiteral(IntegerLiteralNode* node);
	Object* evalFloatLiteral(FloatLiteralNode* node);
	Object* evalMeasureLiteral(MeasureLiteralNode* node);
	Object* evalColorLiteral(ColorLiteralNode* node);
	Object* evalArrayLiteral(ArrayLiteralNode* node);
	Object* evalStringLiteral(StringLiteralNode* node);

};