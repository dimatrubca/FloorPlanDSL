#include <map>

#include "../Objects/Object.h"
#include "../Objects/Room.h"
#include "../Objects/Bed.h"
#include "Evaluator.h"
#include "../Utils.h"

Object* setSize(Object* widthNode, Object* heightNode) {
	auto width = dynamic_cast<IntegerLiteralNode*>(widthNode);
	auto height = dynamic_cast<IntegerLiteralNode*>(heightNode);

	if (!width || !height) {
		std::cout << "INVALID setSize parameters";
		// raise error
	}

	return nullptr;
}
	
Evaluator::Evaluator(Enviroment* env) {
	this->env = env;
}

Object* Evaluator::eval(Node* node) {
	if (auto program = dynamic_cast<ProgramNode*>(node)) {
		return evalProgram(program);
	}

	if (auto statement = dynamic_cast<AssignmentStatementNode*>(node)) {
		return evalAssignmentStatement(statement);
	}

	if (auto statement = dynamic_cast<DeclarationStatementNode*>(node)) {
		return evalDeclarationStatement(statement);
	}

	if (auto statement = dynamic_cast<ExpressionStatementNode*>(node)) {
		return evalExpressionStatement(statement);
	}

	if (auto statement = dynamic_cast<HeaderStatementNode*>(node)) {
		return evalHeaderStatement(statement);
	}

	if (auto prefixNode = dynamic_cast<PrefixExpressionNode*>(node)) {
		Object* right  = eval(prefixNode->right);

		return evalPrefixExpression(prefixNode->op, right);
	}

	if (auto infixNode = dynamic_cast<InfixExpressionNode*>(node)) {
		Object* left = eval(infixNode->left);
		Object* right = eval(infixNode->right);

		return evalInfixExpression(infixNode->op, left, right);
	}

	if (auto intNode = dynamic_cast<IntegerLiteralNode*>(node)) {
		return evalIntLiteral(intNode);
	}

	if (auto arrayNode = dynamic_cast<ArrayLiteralNode*>(node)) {
		return evalArrayLiteral(arrayNode);
	}
	
	if (auto structure = dynamic_cast<StructureStatementNode*>(node)) {

		return evalStructureStatement(structure);
	}
	/*
	if (auto prop = dynamic_cast<PropertyNode*>(node)) {
		return evalPropertyNode(prop);
	}*/

	if (auto identNode = dynamic_cast<IdentifierNode*>(node)) {
		return evalIdentifier(identNode);
	}


	if (auto floatNode = dynamic_cast<FloatLiteralNode*>(node)) {
		return evalFloatLiteral(floatNode);
	}

	if (auto measureNode = dynamic_cast<MeasureLiteralNode*>(node)) {
		return evalMeasureLiteral(measureNode);
	}

	if (auto stringNode = dynamic_cast<StringLiteralNode*>(node)) {
		return evalStringLiteral(stringNode);
	}

	if (auto colorNode = dynamic_cast<ColorLiteralNode*>(node)) {
		return evalColorLiteral(colorNode);
	}

	if (auto indexNode = dynamic_cast<IndexExpressionNode*>(node)) {
		return evalIndexExpression(indexNode);
	}
	return nullptr;

}


Object* Evaluator::evalProgram(ProgramNode* program) {
	Object* result = nullptr;

	for (StatementNode* statement : program->statements) {
		result = eval(statement);
	}

	return result;
};

Object* Evaluator::evalExpressionStatement(ExpressionStatementNode* stmt) {
	return eval(stmt->expression);
}

Object* Evaluator::evalHeaderStatement(HeaderStatementNode* stmt) {
	env->windowWidth = stmt->width;
	env->windowHeight = stmt->height;

	return nullptr;
}

Object* Evaluator::evalDeclarationStatement(DeclarationStatementNode* stmt) {
	Object* valueObj = eval(stmt->value);
	std::string varName = stmt->varName->value;

	if (valueObj->getType() == ERROR_OBJ) {
		return valueObj; // raise error
	}

	env->set(varName, valueObj);
	return valueObj;
}

Object* Evaluator::evalAssignmentStatement(AssignmentStatementNode* stmt) {
	Object* valueObj = eval(stmt->value);
	std::string varName = stmt->varName->value;

	if (valueObj->getType() == ERROR_OBJ) {
		// raise error
	}


	if (!(env->exists(varName)) || env->get(varName)->getType() != valueObj->getType()) {
		// raise error
	}

	env->set(varName, valueObj);

	return valueObj;
};

Object* Evaluator::evalStructureStatement(StructureStatementNode* structureNode) {
	std::map<TokenType, Object*> params;

	for (PropertyNode* prop : structureNode->properties) {
		TokenType propName = prop->name;
		Object* propValue = eval(prop->value);

		params[propName] = propValue;
	}

	// test any
	if (structureNode->structureType == ROOM) {
		Room* room = new Room(params);

		env->set(room->id, room);
		return room;
	}
	else if (structureNode->structureType == WALL) {
		Wall* wall = new Wall(params);

		env->set(wall->id, wall);
		return wall;
	}
	else if (structureNode->structureType == BED) {
		Bed* bed = new Bed(params, env->rooms);

		env->set(bed->id, bed);
		return bed;
	}
	else if (structureNode->structureType == DOOR) {
		Door* door = new Door(params, env->rooms);

		env->set(door->id, door);
		return door;
	}
	else if (structureNode->structureType == WINDOW) {
		Window* window = new Window(params, env->rooms);

		env->set(window->id, window);
		return window;
	}
	else if (structureNode->structureType == TABLE) {
		Table* table = new Table(params, env->rooms);

		env->set(table->id, table);
		return table;
	}
	else if (structureNode->structureType == CHAIR) {
		Chair* chair = new Chair(params, env->rooms);

		env->set(chair->id, chair);
		return chair;
	}
};

std::pair<TokenType, Object*> Evaluator::evalPropertyNode(PropertyNode* propertyNode) {

	return { propertyNode->name, eval(propertyNode->value) };
}

Object* Evaluator::evalPrefixExpression(TokenType op, Object* right) {
	if (op == MINUS) {
		return evalMinusPrefixOperatorExpression(right);
	}
	else {
		//error
	}
}

Object* Evaluator::evalMinusPrefixOperatorExpression(Object* right) {
	if (right->getType() == INTEGER_OBJ) {
		int value = dynamic_cast<Integer*>(right)->value;
		return new Integer(-value);
	}

	if (right->getType() == FLOAT_OBJ) {
		float value = dynamic_cast<FloatObject*>(right)->value;
		return new FloatObject(-value);
	}

	return nullptr;
}


Object* Evaluator::evalInfixExpression(TokenType op, Object* left, Object* right) {
	Object* leftNew;
	Object* rightNew;

	// if one float or measure type -> convert int to float
	bool flag = (left->getType() == FLOAT_OBJ || left->getType() == MEASURE
		|| right->getType() == FLOAT_OBJ || right->getType() == MEASURE);

	if (flag) {
		if (left->getType() == INTEGER_OBJ) left = FloatObject::intToFloat(dynamic_cast<Integer*>(left));
		if (right->getType() == INTEGER_OBJ) right = FloatObject::intToFloat(dynamic_cast<Integer*>(right));
	}


	if (left->getType() == INTEGER_OBJ && right->getType() == INTEGER_OBJ) {
		Integer* leftInt = dynamic_cast<Integer*>(left);
		Integer* rightInt = dynamic_cast<Integer*>(right);

		return evalIntegerInfixExpression(op, leftInt, rightInt);
	}
	else if (left->getType() == FLOAT_OBJ && right->getType() == FLOAT_OBJ) {
		FloatObject* leftFloat = dynamic_cast<FloatObject*>(left);
		FloatObject* rightFloat = dynamic_cast<FloatObject*>(right);

		return evalFloatInfixExpression(op, leftFloat, rightFloat);
	}
	else if (left->getType() == MEASURE_OBJ && right->getType() == MEASURE_OBJ) {
		Measure* leftMeasure = dynamic_cast<Measure*>(left);
		Measure* rightMeasure = dynamic_cast<Measure*>(right);

		return evalMeasureInfixExpression(op, leftMeasure, rightMeasure);
	}
	else if (left->getType() == STRING_OBJ && right->getType() == STRING_OBJ) {
		String* leftString = dynamic_cast<String*>(left);
		String* rightString = dynamic_cast<String*>(right);

		return evalStringInfixExpression(op, leftString, rightString);
	}
};

Object* Evaluator::evalIntegerInfixExpression(TokenType op, Integer* left, Integer* right) {
	if (op == PLUS) {
		return *left + *right;

	}
	else if (op == MINUS) {
		return *left - *right;

	}
	else if (op == ASTERISK) {
		return *left * *right;
	}
	else if (op == SLASH) { // TODO: division by zero exception
		return new FloatObject(1.0 * left->value / right->value);
	}
	else return nullptr;
};

Object* Evaluator::evalFloatInfixExpression(TokenType op, FloatObject* left, FloatObject* right) {
	if (op == PLUS) {
		return new FloatObject(left->value + right->value);
	}
	else if (op == MINUS) {
		return new FloatObject(left->value - right->value);
	}
	else if (op == ASTERISK) {
		return new FloatObject(left->value * right->value);
	}
	else if (op == SLASH) { // TODO: division by zero exception
		return new FloatObject(left->value / right->value);
	}
	else return nullptr;
}

Object* Evaluator::evalStringInfixExpression(TokenType op, String* left, String* right) {
	if (op == PLUS) {
		return new String(left->value + right->value);
	}
	else return nullptr;
}

Object* Evaluator::evalMeasureInfixExpression(TokenType op, Measure* left, Measure* right) {
	if (op == PLUS) {
		return new Measure(left->value + right->value);
	}
	else if (op == MINUS) {
		return new Measure(left->value - right->value);
	}
	else return nullptr;
}

Object* Evaluator::evalIdentifier(IdentifierNode* node) {
	return env->get(node->value);
}

Object* Evaluator::evalIntLiteral(IntegerLiteralNode* node) {
	return new Integer(node->value);
}

Object* Evaluator::evalFloatLiteral(FloatLiteralNode* node) {
	return new FloatObject(node->value);
}

Object* Evaluator::evalStringLiteral(StringLiteralNode* node) {
	return new String(node->value);
}

Object* Evaluator::evalColorLiteral(ColorLiteralNode* node) {
	return new Color(node->value);
}


Object* Evaluator::evalMeasureLiteral(MeasureLiteralNode* node) {
	Object* valueObj = eval(node->valueExpr);
	float value = -100;

	if (auto intObj = dynamic_cast<Integer*>(valueObj)) {
		value = intObj->value;
	}
	
	if (auto floatObj = dynamic_cast<FloatObject*>(valueObj)) {
		value = floatObj->value;
	}

	/*if (value == -100) {
		std::cout << "eval measure literal value not identifier";
	}*/

	return new Measure(value, node->unit);
}

Object* Evaluator::evalIndexExpression(IndexExpressionNode* node) {
	auto arr = eval(node->left);
	auto index = eval(node->index);

	auto arrObject = dynamic_cast<Array*>(arr);
	auto indexInt = dynamic_cast<Integer*>(index);

	if (indexInt->value >= arrObject->elements.size()) {
		std::cout << "ERROR::index out of bounds\n";
		return 0;
	}

	return arrObject->elements.at(indexInt->value);
}


Object* Evaluator::evalArrayLiteral(ArrayLiteralNode* node) {
	Array* arrayObj = new Array();

	for (auto elem : node->elements) {
		arrayObj->elements.push_back(eval(elem));
	}

	return arrayObj;
}