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
	Object* evalAssignmentStatement(AssignmentStatementNode* stmt);
	Object* evalDeclarationStatement(DeclarationStatementNode* stmt);
	Object* evalStructureStatement(StructureStatementNode* structure);
	Object* evalPropertyNode(PropertyNode* prop);
	Object* evalPrefixExpression(PrefixExpressionNode* expr);
	Object* evalInfixExpression(InfixExpressionNode* expr);
	Object* evalIntegerInfixExpression(InfixExpressionNode* expr);
	Object* evalMeasureInfixExpression(InfixExpressionNode* expr);
	Object* evalIndexExpression(IndexExpressionNode* expr);
	Object* evalIdentifier(IdentifierNode* ident);
	Object* evalIntLiteral(IntegerLiteralNode* node);
	Object* evalFloatLiteral(FloatLiteralNode* node);
	Object* evalMeasureLiteral(MeasureLiteralNode* node);
	Object* evalColorLiteral(ColorLiteralNode* node);
	Object* evalArrayLiteral(ArrayLiteralNode* node);
	Object* evalStringLiteral(StringLiteralNode* node);

};