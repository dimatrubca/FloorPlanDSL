#include <map>

#include "Object.h"
#include "Evaluator.h"

Object* Evaluator::eval(Node* node) {
	if (node->getType() == AstNodeType::IntLiteral) {
		IntegerLiteral& intNode = dynamic_cast<IntegerLiteral&>(*node);

		return intNode.value;
	} 

	switch (node->getType())
	{
	case AstNodeType::InfixExpression:
		InfixExpression& infixNode = dynamic_cast<IntegerLiteral&>(*node);

		auto left = eval(infixNode.left);
		auto right = eval(infixNode.right);
		
		return evalInfixExpression(infixNode.opToken, left, right);
	case AstNodeType::IntLiteral:
		IntegerLiteral& intNode = dynamic_cast<IntegerLiteral&>(*node);

		return new Integer(intNode.value);
	break;
	case AstNodeType::FloatLiteral:
		FloatLiteral& floatNode = dynamic_cast<FloatLiteral&>(*node);

		return new Float(floatNode.value);
	case AstNodeType::StringLiteral:
		break;
	case AstNodeType::MeasureLiteral:
		MeasureLiteral& measureNode = dynamic_cast<MeasureLiteral&>(*node);
		Object* value = eval(measureNode.valueExpr);

		return new Measure(measureNode.valueExpr, measureNode.unit);
	case AstNodeType::ColorLiteral:
		break;

	case AstNodeType::Structure:
		StructureNode& structureNode = dynamic_cast<StructureNode&>(*node);
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