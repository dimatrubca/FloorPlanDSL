#include "Parser.h"


Parser::Parser(Lexer& lexer) : lexer(lexer) {
	advance();
	advance();

	// prefix functions
	prefixFuncs[IDENTIFIER] = &Parser::parseIdentifier;
	prefixFuncs[INT_LITERAL] = &Parser::parseIntegerLiteral;
	prefixFuncs[FLOAT_LITERAL] = &Parser::parseFloatLiteral;
	prefixFuncs[COLOR_LITERAL] = &Parser::parseColorLiteral;
	prefixFuncs[STRING_LITERAL] = &Parser::parseStringLiteral;
	prefixFuncs[LBRACE] = &Parser::parseArrayLiteral;
	prefixFuncs[MINUS] = &Parser::parsePrefixExpression;
	prefixFuncs[LPAREN] = &Parser::parseGroupedExpression;

	// infix functions
	infixFuncs[MINUS] = &Parser::parseInfixExpression;
	infixFuncs[PLUS] = &Parser::parseInfixExpression;
	infixFuncs[SLASH] = &Parser::parseInfixExpression;
	infixFuncs[ASTERISK] = &Parser::parseInfixExpression;
	infixFuncs[LPAREN] = &Parser::parseCallExpression;
	infixFuncs[LBRACKET] = &Parser::parseIndexExpression;

	// precendences 
	precendences[PLUS] = Precendence::SUM;
	precendences[MINUS] = Precendence::SUM;
	precendences[SLASH] = Precendence::PRODUCT;
	precendences[ASTERISK] = Precendence::PRODUCT;
	precendences[LBRACKET] = Precendence::INDEX;
	precendences[LPAREN] = Precendence::CALL;
}

ProgramNode* Parser::parseProgram() {
	ProgramNode* program = new ProgramNode(currToken);
	StatementNode* statement;

	while (currToken.type != END) {
		if (isDataType(currToken)) {
			statement = parseDeclarationStatement();
		}
		else if (isStructureName(currToken)) {
			statement = parseStructureDeclaration();
		}
		else if (currToken.type == IDENTIFIER && peekToken.type == ASSIGN) {
			statement = parseAssignmentStatement();
		}
		else if (currToken.type == EXCLAM_MARK) {
			statement = parseHeader();
		}
		else {
			statement = parseExpressionStatement();
		}

		if (statement) program->statements.push_back(statement);
		advance();
	}

	return program;
}

HeaderStatementNode* Parser::parseHeader() {
	HeaderStatementNode* header = new HeaderStatementNode(currToken);

	// TODO: check errors
	if (peekToken.type != INT_LITERAL) {
		return nullptr;
	}

	advance();

	header->width = stoi(currToken.literal);

	if (peekToken.literal != "x") {
		return nullptr;
	}
	advance();

	if (peekToken.type != INT_LITERAL) {
		return nullptr;
	}
	advance();

	header->height = stoi(currToken.literal);

	return header;
}

ExpressionStatementNode* Parser::parseExpressionStatement() {
	ExpressionStatementNode* statement = new ExpressionStatementNode(currToken);

	statement->expression = parseExpression(Precendence::LOWEST, 0);

	if (!expectPeek(SEMICOLON)) {
		return nullptr;
	}

	return statement;
}

StatementNode* Parser::parseDeclarationStatement() {
	DeclarationStatementNode* statement = new DeclarationStatementNode(currToken);
	
	statement->dataType = currToken.type;
	if (!expectPeek(IDENTIFIER)) return nullptr;
	statement->varName = new IdentifierNode(currToken, currToken.literal);
	if (!expectPeek(ASSIGN)) return nullptr;
	advance();
	statement->value = parseExpression(Precendence::LOWEST, 0);
	if (!expectPeek(SEMICOLON)) return nullptr;

	return statement;
}

AssignmentStatementNode* Parser::parseAssignmentStatement() {
	AssignmentStatementNode* statement = new AssignmentStatementNode(currToken);

	statement->varName = new IdentifierNode(currToken, currToken.literal);

	if (!expectPeek(ASSIGN)) return nullptr;
	advance();
	statement->value = parseExpression(Precendence::LOWEST, 0);
	if (!expectPeek(SEMICOLON)) return nullptr;

	return statement;
}

StructureStatementNode* Parser::parseStructureDeclaration() {
	StructureStatementNode* structure = new StructureStatementNode(currToken, currToken.type);

	if (!expectPeek(LPAREN)) return nullptr;
	while (peekToken.type != RPAREN) {
		requirePeekPropertyOf(ROOM);
		PropertyNode* prop = parsePropertyNode();
		structure->properties.push_back(prop);
		
		if (peekToken.type != RPAREN && !expectPeek(COMMA)) return nullptr;
	}

	if (!expectPeek(RPAREN)) return nullptr;
	if (!expectPeek(SEMICOLON)) return nullptr;

	return structure;
}

PropertyNode* Parser::parsePropertyNode() {
	PropertyNode* prop = new PropertyNode(currToken, currToken.type);

	if (!expectPeek(COLON)) return nullptr;
	advance();
	prop->value = parseExpression(Precendence::LOWEST, 0);

	return prop;
}


ExpressionNode* Parser::parseIdentifier() {
	return new IdentifierNode(currToken, currToken.literal);
}

ExpressionNode* Parser::parseIntegerLiteral() {
	int value;

	try {
		value = stoi(currToken.literal);
	}
	catch (const std::exception& e) {
		error(currToken, "Coudn't parse " + currToken.literal + " as integer");
		return nullptr;
	}

	return new IntegerLiteralNode(currToken, value);
}

// TODO: check currToken.literal for string
ExpressionNode* Parser::parseStringLiteral() {
	return new StringLiteralNode(currToken, currToken.literal);
}

ExpressionNode* Parser::parseFloatLiteral() {
	double value;

	try {
		value = stof(currToken.literal);
	}
	catch (const std::exception& e) {
		error(currToken, "Coudn't parse " + currToken.literal + " as float");
		return nullptr;
	}

	return new FloatLiteralNode(currToken, value);
}

ExpressionNode* Parser::parseMeasureLiteral() {
	ExpressionNode* valueExpr = parseExpression(Precendence::LOWEST, 1);
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
	ExpressionNode* exp = parseExpression(Precendence::LOWEST,0);
	if (!expectPeek(RPAREN)) return nullptr;

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

ExpressionNode* Parser::parseExpression(Precendence precendence, bool parsingMeasure) {
	if (!hasKey(prefixFuncs, currToken.type)) {
		error(currToken, "No prefix function registered for token");
		return NULL;
	}

	std::function<ExpressionNode* (Parser& const)> prefixFunc = prefixFuncs[currToken.type];
	// check exception
	if (!parsingMeasure && (currToken.type == INT_LITERAL || currToken.type == FLOAT_LITERAL) && isMeasureUnit(peekToken)) prefixFunc = &Parser::parseMeasureLiteral;

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
	expr->right = parseExpression(Precendence::PREFIX, 0);
	
	return expr;
}

ExpressionNode* Parser::parseInfixExpression(ExpressionNode* left) {
	auto expression = new InfixExpressionNode(currToken, currToken.type, left);
	auto prec = currentPrecendence();

	advance();
	expression->right = parseExpression(prec, 0);

	return expression;
}

ExpressionNode* Parser::parseCallExpression(ExpressionNode* function) {
	auto expression = new CallExpressionNode(currToken, function);
	expression->arguments = parseExpressionList(RPAREN);
	
	return expression;
}


// double check this 
ExpressionNode* Parser::parseArrayLiteral() {
	auto arrayNode = new ArrayLiteralNode(currToken);

	arrayNode->elements = parseExpressionList(RBRACE);

	return arrayNode;
}


// '(1, a+2, 3)', {1, 2, 3}
std::vector<ExpressionNode*> Parser::parseExpressionList(TokenType end) {
	std::vector<ExpressionNode*> args;

	advance();

	if (currToken.type == end) {
		return args;
	}

	args.push_back(parseExpression(Precendence::LOWEST, 0));

	while (peekToken.type == COMMA) {
		advance();
		advance();
		args.push_back(parseExpression(Precendence::LOWEST, 0));
	}

	if (!expectPeek(end)) {
		return std::vector<ExpressionNode*>();
	}

	return args;
}

ExpressionNode* Parser::parseIndexExpression(ExpressionNode* left) {
	auto expression = new IndexExpressionNode(currToken, left);

	advance();
	expression->index = parseExpression(Precendence::LOWEST, 0);
	if (!expectPeek(RBRACKET)) return nullptr;

	return expression;
}


bool Parser::expectPeek(TokenType type) {
	advance();

	if (currToken.type != type) {
		error(currToken, "Expected next token to be " + type + ", got " + currToken.type);
		return false;
	}

	return true;
}

void Parser::requirePeekPropertyOf(TokenType structureType) {
	advance();
}
 
void Parser::error(Token token, std::string message) {
	ParserError* err = new ParserError(token, message);
	errors.push_back(err);
}

void Parser::advance() {
	currToken = peekToken;
	peekToken = lexer.NextToken();
	//std::cout << "advancing...\n";
	//std::cout << std::string(currToken)<<'\n';
}

std::vector<ParserError*> Parser::getErrors() {
	return errors;
}