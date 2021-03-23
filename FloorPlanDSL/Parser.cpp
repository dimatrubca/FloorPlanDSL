#include "Parser.h"
#include "ParserError.h"

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

ProgramNode* Parser::parseProgram() {
	ProgramNode* program = new ProgramNode(currToken);
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
	DeclarationStatementNode* statement = new DeclarationStatementNode(currToken);

	statement->varType = parseIdentifier();
	requirePeek(IDENTIFIER);
	statement->varName = parseIdentifier();
	requirePeek(ASSIGN);
	advance();
	statement->value = parseExpression(Precendence::LOWEST);
	requirePeek(SEMICOLON);

	return statement;
}

AssignmentStatementNode* Parser::parseAssignmentStatement() {
	AssignmentStatementNode* statement = new AssignmentStatementNode(currToken);

	requirePeek(IDENTIFIER); // advances, checks, throws error
	statement->variable = new IdentifierNode(currToken, currToken.literal);

	requirePeek(ASSIGN);
	advance();
	statement->value = parseExpression(Precendence::LOWEST);
	requirePeek(SEMICOLON);

	return statement;
}

StructureStatementNode* Parser::parseStructureDeclaration() {
	StructureStatementNode* structure = new StructureStatementNode(currToken, currToken.type);

	requirePeek(LPAREN);
	while (peekToken.type != RPAREN) {
		requirePeek(IDENTIFIER);
		PropertyNode* prop = parsePropertyNode();
		structure->properties.push_back(prop);
		
		if (peekToken.type != RPAREN) requirePeek(COMMA);
	}

	advance();
	return structure;
}

PropertyNode* Parser::parsePropertyNode() {
	PropertyNode* prop = new PropertyNode(currToken, currToken.type);

	requirePeek(COLON);
	advance();
	prop->value = parseExpression(Precendence::LOWEST);

	return prop;
}


ExpressionNode* Parser::parseIdentifier() {
	return new IdentifierNode(currToken, currToken.literal);
}

ExpressionNode* Parser::parseIntegerLiteral() {
	return new IntegerLiteralNode(currToken, std::stoi(currToken.literal));
}

// TODO: check currToken.literal for string
ExpressionNode* Parser::parseStringLiteral() {
	return new StringLiteralNode(currToken, currToken.literal);
}

ExpressionNode* Parser::parseFloatLiteral() {
	return new FloatLiteralNode(currToken, std::stof(currToken.literal));
}

ExpressionNode* Parser::parseMeasureLiteral() {
	ExpressionNode* valueExpr = parseExpression(Precendence::LOWEST);
	advance(); // require measure unit()
	TokenType unitType = currToken.type;

	return new MeasureLiteralNode(currToken, valueExpr, unitType);
}

ExpressionNode* Parser::parseColorLiteral() {
	std::string hexValue = currToken.literal.substr(1);

	return new ColorLiteralNode(currToken, hexValue);
}

ExpressionNode* Parser::parseGroupedExpression() {
	advance();
	ExpressionNode* exp = parseExpression(Precendence::LOWEST);
	requirePeek(RPAREN);

	return exp;
}

Precendence Parser::currentPrecendence() {
	if (!hasKey(precendences, currToken.type)) return Precendence::LOWEST;
	return precendences[currToken.type];
}


Precendence Parser::peekPrecendence() {
	if (!hasKey(precendences, peekToken.type)) return Precendence::LOWEST; // check return value
	return precendences[peekToken.type];
}

ExpressionNode* Parser::parseExpression(Precendence precendence) {
	if (!hasKey(prefixFuncs, currToken.type)) {
		error(currToken, "No prefix function registered for token");
		return NULL;
	}

	std::function<ExpressionNode* (Parser& const)> prefixFunc = prefixFuncs[currToken.type];
	ExpressionNode* left = prefixFunc(*this);

	while (peekToken.type != SEMICOLON && precendence < peekPrecendence()) {
		if (!hasKey(infixFuncs, peekToken.type)) {
			return left;
		}

		auto infixFunc = infixFuncs[peekToken.type];
		advance();
		left = infixFunc(*this, left);
	}

	return left;
}

ExpressionNode* Parser::parsePrefixExpression() {
	auto expr = new PrefixExpressionNode(currToken, currToken.type);
	advance();
	expr->right = parseExpression(Precendence::PREFIX);
	
	return expr;
}

ExpressionNode* Parser::parseInfixExpression(ExpressionNode* left) {
	auto expression = new InfixExpressionNode(currToken, currToken.type, left);
	auto prec = currentPrecendence();

	advance();
	expression->right = parseExpression(prec);

	return expression;
}

// double check this 
ExpressionNode* Parser::parseArrayLiteral() {
	auto arrayNode = new ArrayLiteralNode(currToken);

	advance();
	// inifinite loop check
	while (currToken.type != RBRACE) {
		auto expression = parseExpression(Precendence::LOWEST);
		arrayNode->elements.push_back(expression);

		advance();
		if (currToken.type != COMMA && currToken.type != RBRACE) {
			error(currToken, "Invalid character inside array declaration");
		}

		if (currToken.type == COMMA) advance();
	}

	return arrayNode;
}

ExpressionNode* Parser::parseIndexExpression(ExpressionNode* left) {
	auto expression = new IndexExpressionNode(currToken, left);

	advance();
	expression->index = parseExpression(Precendence::LOWEST);
	requirePeek(RBRACKET);

	return expression;
}


void Parser::requirePeek(TokenType type) {
	advance();

	if (currToken.type != type) {
		error(currToken, "msg");
	}
}

void Parser::error(Token token, std::string message) {
	throw new ParserError(token, message);
}