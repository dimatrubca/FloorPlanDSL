#include <sstream>
#include "AST.h"

std::map<AstNodeType, std::string>  AstNodeTypeToString = {
	{AstNodeType::Program, "Program"},
	{AstNodeType::AssignmentStatement, "AssignmentStatement"},
	{AstNodeType::DeclarationStatement, "DeclarationStatement"},
	{AstNodeType::ExpressionStatement, "ExpressionStatement"},
	{AstNodeType::Structure, "Structure"},
	{AstNodeType::Property, "Property"},
	{AstNodeType::PrefixExpression, "PrefixExpression"},
	{AstNodeType::InfixExpression, "InfixExpression"},
	{AstNodeType::IndexExpression, "IndexExpression"},
	{AstNodeType::Identifier, "Identifier"},
	{AstNodeType::IntLiteral, "IntLiteral"},
	{AstNodeType::FloatLiteral, "FloatLiteral"},
	{AstNodeType::MeasureLiteral, "MeasureLiteral"},
	{AstNodeType::ColorLiteral, "ColorLiteral"},
	{AstNodeType::ArrayLiteral, "ArrayLiteral"},
	{AstNodeType::StringLiteral, "StringLiteral"}
 };



std::string ProgramNode::toString() {
	std::stringstream ss;
	
	for (auto it = statements.begin(); it != statements.end(); ++it) {
		ss << (*it)->toString();
		if (it + 1 != statements.end()) ss << '\n';
	}

	return ss.str();
};

std::string AssignmentStatementNode::toString() {
	std::stringstream ss;

	ss << varName->value << " = " << value->toString();
	return ss.str();
}

std::string DeclarationStatementNode::toString() {
	std::stringstream ss;

	ss << dataType << " " << varName->value << " = " << value->toString();

	return ss.str();
}

std::string StructureStatementNode::toString() {
	std::stringstream ss;

	ss << structureType << "(\n";
	for (auto propNode : properties) {
		ss << propNode->toString() << "\n";
	}
	ss << ")";

	return ss.str();
}

std::string ExpressionStatementNode::toString() {
	std::stringstream ss;

	ss << expression->toString() << ";";

	return ss.str();
}

std::string PropertyNode::toString() {
	std::stringstream ss;

	ss << "\t" << name << ": " << value->toString();
	return ss.str();
}


std::string PrefixExpressionNode::toString() {
	std::stringstream ss;

	ss << "(" + op << right->toString() + ")";
	return ss.str();
}

std::string InfixExpressionNode::toString() {
	std::stringstream ss;

	ss << "(" + left->toString() << + " " + op << + " " + right->toString() + ")";
	return ss.str();

}

std::string IndexExpressionNode::toString() {
	std::stringstream ss;

	ss << left->toString() << "[" << index->toString() << "]";
	return ss.str();
}

std::string CallExpressionNode::toString() {
	std::stringstream ss;

	ss << function->toString() << "(";

	bool first = true;
	for (ExpressionNode* arg : arguments) {
		if (!first) ss << ", ";
		ss << arg->toString();
		first = false;
	}
	ss << ")";

	return ss.str();
}

std::string ArrayLiteralNode::toString() {
	std::stringstream ss;

	ss << "{";
	for (auto it = elements.begin(); it != elements.end(); it++) {
		ss << (*it)->toString();
		if (it + 1 != elements.end()) ss << ", ";
	}
	ss << "}";

	return ss.str();
}

