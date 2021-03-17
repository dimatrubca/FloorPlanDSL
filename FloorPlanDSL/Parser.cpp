#include "Parser.h"

Parser::Parser(Lexer& lexer) : lexer(lexer) {
	// prefix functions
	prefixFuncs[IDENTIFIER] = parseIdentifier;
	prefixFuncs[INT_LITERAL] = parseIntegerLiteral;
	prefixFuncs[STRING_LITERAL] = parseStringLiteral;
	prefixFuncs[LBRACE] = parseArrayLiteral;
	prefixFuncs[MINUS] = parsePrefixExpression;
	prefixFuncs[LPAREN] = parseGroupedExpression;

	// infix functions
	infixFuncs[MINUS] = parseInfixExpression;
	infixFuncs[PLUS] = parseInfixExpression;
	infixFuncs[SLASH] = parseInfixExpression;
}

Program* Parser::parseProgram() {
	Program* program = new Program();
	StatementNode* statement;
	advance();

	while (currToken.type != END) {
		if (isDataType(currToken)) {
			statement = parseDeclarationStatement();
		}
		else if (isStructureName(currToken)) {
			statement = parseStructureDeclaration();
		}
		else if (currToken.type == IDENTIFIER) {
			statement = parseAssignmentStatement();
		}
		else {

		}

		program->statements.push_back(statement);
	}

	return program;
}

StatementNode* Parser::parseDeclarationStatement() {
	DeclarationStatement* statement = new DeclarationStatement();

	statement->token = currToken;
	statement->varType = parseIdentifier();
	statement->varName = parseIdentifier();
	statement->value = parseExpression(Precendence::LOWEST);

	return statement;
}

ExpressionNode* Parser::parseIdentifier() {
	return new Identifier(currToken, currToken.literal);
}

ExpressionNode* Parser::parseIntegerLiteral() {
	return new IntegerLiteral(currToken, std::stoi(currToken.literal));
}

// TODO: check currToken.literal for string
ExpressionNode* Parser::parseStringLiteral() {
	return new StringLiteral(currToken, currToken.literal);
}

ExpressionNode* Parser::parseFloatLiteral() {
	return new FloatLiteral(currToken, std::stof(currToken.literal));
}

ExpressionNode* Parser::parseMeasureLiteral() {
	ExpressionNode* valueExpr = parseExpression(Precendence::LOWEST);
	ExpressionNode* unit = parseIdentifier();

	return new MeasureLiteral(valueExpr, unit);
}

ExpressionNode* Parser::parseColorLiteral() {
	std::string hexValue = currToken.literal.substr(1);

	return new ColorLiteral(currToken, hexValue);
}

ExpressionNode* Parser::parseGroupedExpression() {
	requireToken(LPAREN);
	ExpressionNode* expression = parseExpression(Precendence::LOWEST);
	requireToken(RPAREN);
}


Structure* Parser::parseStructureDeclaration() {
	Structure* structure = new Structure();

	// require structure identifier
	if (structures.find(currToken.type) == structures.end()) {
		// raise Exception
	}

	structure->token = currToken;
	structure->name = parseIdentifier();

	do {
		PropertyNode* prop = new PropertyNode();
		prop->name = parseIdentifier();
		prop->value = parseExpression(Precendence::LOWEST); // TODO: handle array literal

		structure->properties.push_back(prop);
	} while (currToken.type == COMMA);

	return structure;
}

Precendence Parser::peekPrecendence() {
	if (!hasKey(precendences, peekToken.type)) return Precendence::LOWEST; // check return value
	return precendences[peekToken.type];
}

ExpressionNode* Parser::parseExpression(Precendence prec) {
	if (!hasKey(prefixFuncs, currToken.type)) {
		// add error
		return NULL;
	}

	auto func = prefixFuncs[currToken.type];
	ExpressionNode* left = func(left);

	while (true) {
		Expression* expression = parseInfixExpression(left);
		// call infix functions with left
	}

	return left;
}


void Parser::requireSymbol(char symbol) {
	advance();
	if (tokenizer.tokenType() == TokenType::Symbol && tokenizer.symbol() == symbol) {
		// print
	}
	else {
		error("'" + std::string(1, symbol) + "'");
	}
}

void Parser::requireToken(TokenType type) {
	advance();

	if (currToken.type != type) {
		addError("msg");
	}
}