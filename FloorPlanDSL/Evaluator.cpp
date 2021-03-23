#include <map>

#include "Object.h"
#include "Evaluator.h"


Object* Evaluator::eval(Node* node) {
	if (auto program = dynamic_cast<ProgramNode*>(node)) {
		evalProgram(program);
	}

	if (auto statement = dynamic_cast<AssignmentStatementNode*>(node)) {
		return evalAssignmentStatement(statement);
	}

	if (auto statement = dynamic_cast<DeclarationStatementNode*>(node)) {
		return evalDeclarationStatement(statement);
	}

	if (auto structure = dynamic_cast<StructureStatementNode*>(node)) {

		return evalStructureStatement(structure);
	}

	if (auto prop = dynamic_cast<PropertyNode*>(node)) {
		return evalPropertyNode(prop);
	}

	if (auto intNode = dynamic_cast<IntegerLiteralNode*>(node)) {
		return evalIntLiteral(intNode);
	}

	if (auto floatNode = dynamic_cast<FloatLiteralNode*>(node)) {
		return evalFloatLiteral(floatNode);
	}

	if (auto measureNode = dynamic_cast<MeasureLiteralNode*>(node)) {
		return evalMeasureLiteral(measureNode);
	}

	if (auto colorNode = dynamic_cast<ColorLiteralNode*>(node)) {
		return evalColorLiteral(colorNode);
	}

}


Object* Evaluator::evalProgram(ProgramNode* program) {
	for (StatementNode* statement : program->statements) {
		evalStatement(statement);
	}

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
	std::string varName = stmt->variable->value;

	if (valueObj->getType() == ERROR_OBJ) {
		// raise error
	}


	if (!(env->exists(varName)) || env->get(varName)->getType() != valueObj->getType()) {
		// raise error
	}

	env->set(varName, valueObj);

	return valueObj;
}

Object* Evaluator::evalStructureStatement(StructureStatementNode* structure) {
	std::map<TokenType, Object> params;

	for (PropertyNode* prop : structure->properties) {
		TokenType propName = prop->name;
		Object* propValue = evalExpression(prop->value);

		params.insert({ propName, propValue });
	}

	if (structure->structureType == ROOM) {
		Room* structure = new Room(params);
		// add to enviroment
	}
	else if (structureNode->structureType == WALL) {
		Wall* wall = new Wall(params);
	}

}

std::pair<TokenType, Object> evalProperty(PropertyNode* propertyNode) {

}

Object* Evaluator::evalPrefixExpression(PrefixExpressionNode* node) {
	Object* left = eval(node->right);

	if (left->getType() == INTEGER_OBJ) {
		Integer* intObject = dynamic_cast<Integer*>(left);
		return new Integer(intObject->value);
	}

	if (left->getType() == FLOAT_OBJ) {
		Float* floatObject = dynamic_cast<Float*>(left);
		return new Float(floatObject->value);
	}

	std::cout << "Invalid eval prefix expression";
	return NULL;
}

// template method???
Object* Evaluator::evalInfixExpression(InfixExpressionNode* node) {
	Object* left = eval(node->left);
	Object* right = eval(node->right);

	if (left->getType() == MEASURE_OBJ && right->getType() == MEASURE_OBJ) {
		return evalMeasureInfixExpression(node);
	} else if (left->getType() == INTEGER_OBJ && right->getType() == INTEGER_OBJ) {
		return evalIntegerInfixExpression(node);
	} else {
		std::cout << "Eval infix expression, invalid type";
	}

};

Object* Evaluator::evalIntegerInfixExpression(InfixExpressionNode* node) {
		Integer* left = dynamic_cast<Integer*>(eval(node->left));
		Integer* right = dynamic_cast<Integer*>(eval(node->right));

		if (node->op == PLUS) {
			return *left + *right;

		}
		else if (node->op == MINUS) {
			return *left - *right;
		
		} 
		else if (node->op == ASTERISK) {
			return *left * *right;
		}
};

Object* Evaluator::evalMeasureInfixExpression(InfixExpressionNode* node) {
	Measure* left = dynamic_cast<Measure*>(eval(node->left));
	Measure* right = dynamic_cast<Measure*>(eval(node->right));

	if (node->op == PLUS) {
		return *left + *right;
	}
	else if (node->op == MINUS) {
		return *left - *right;
	}
}

Object* Evaluator::evalIdentifier(IdentifierNode* node) {
	return env->get(node->value);
}

Object* Evaluator::evalIntLiteral(IntegerLiteralNode* node) {
	return new Integer(node->value);
}

Object* Evaluator::evalFloatLiteral(FloatLiteralNode* node) {
	return new Float(node->value);
}

Object* Evaluator::evalMeasureLiteral(MeasureLiteralNode* node) {
	Object* valueObj = eval(node->valueExpr);
	float value = -100;

	if (auto intObj = dynamic_cast<Integer*>(valueObj)) {
		value = intObj->value;
	}
	
	if (auto floatObj = dynamic_cast<Float*>(valueObj)) {
		value = floatObj->value;
	}

	if (value == -100) {
		std::cout << "eval measure literal value not identifier";
	}

	return new Measure(value, node->unit);
}


Object* Evaluator::eval(Node* node) {
	if (node->getType() == AstNodeType::IntLiteral) {
		IntegerLiteralNode& intNode = dynamic_cast<IntegerLiteralNode&>(*node);

		return intNode.value;
	} 

	switch (node->getType())
	{
	case AstNodeType::InfixExpression:
		InfixExpressionNode& infixNode = dynamic_cast<IntegerLiteralNode&>(*node);

		auto left = eval(infixNode.left);
		auto right = eval(infixNode.right);
		
		return evalInfixExpression(infixNode.opToken, left, right);
	case AstNodeType::IntLiteral:
		IntegerLiteralNode& intNode = dynamic_cast<IntegerLiteralNode&>(*node);

		return new Integer(intNode.value);
	break;
	case AstNodeType::FloatLiteral:
		FloatLiteralNode& floatNode = dynamic_cast<FloatLiteralNode&>(*node);

		return new Float(floatNode.value);
	case AstNodeType::StringLiteral:
		break;
	case AstNodeType::MeasureLiteral:
		MeasureLiteralNode& measureNode = dynamic_cast<MeasureLiteralNode&>(*node);
		Object* value = eval(measureNode.valueExpr);

		return new Measure(measureNode.valueExpr, measureNode.unit);
	case AstNodeType::ColorLiteral:
		break;

	case AstNodeType::Structure:
		StructureStatementNode& structureNode = dynamic_cast<StructureStatementNode&>(*node);
		std::map<TokenType, Object> params;

		for (PropertyNode* prop : structureNode.properties) {
			prop->name;
			prop->value;
		}

		if (structureNode.structureType == ROOM) {
			Room* structure = new Room(params);
			// add to enviroment
		}
		else if (structureNode.structureType == WALL) {
			Wall* wall = new Wall(params);
		}

		/*	else if (structureNode.structureType == DOOR) {
			Door* door = new Door(params);
			
		}
		else if (structureNode.structureType == ELEVATOR) {
			Elevator* elevator = new Elevator(params);
		}
		else if (structureNode.structureType == BED) {
		
		}
		else if (structureNode.structureType == TABLE) {
		
		}
		else if (structureNode.structureType == CHAIR) {
		
		}
		else {
		
		}*/


		// iterate over properties 
		// get structure type
		break;
	default:
		break;
	}


	
}

Object* Evaluator::evalInfixExpression(Object* left, Object* right, TokenType op) {
	if (left->getType() == STRING_OBJ && right->getType() == STRING_OBJ) {
		return evalStringInfixExpression(left, right, op);
	}
	else if (left->getType() == MEASURE_OBJ && left->getType() == MEASURE_OBJ) {
		return evalMeasureInfixExpression(left, right, op);
	}
	else if (left->getType() == INTEGER_OBJ && right->getType() == INTEGER_OBJ) {
		return evalIntegerInfixExpression(left, right, op);
	}
	else if (left->getType() == FLOAT_OBJ && right->getType() == FLOAT_OBJ) {
		return evalFloatInfixExpressino(left, right, op);
	}
};

Object* Evaluator::evalMeasureInfixExpression(Object* left, Object* right, TokenType op) {
		
};

Object* Evaluator::evalStringInfixExpression(Object* left, Object* right, TokenType op) {
	
}

Object* Evaluator::evalFloatInfixExpression(Object* left, Object* right, TokenType op) {
	if (op == "+") return new Integer()
}