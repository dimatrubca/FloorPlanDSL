#include <stringapiset.h>
#include <codecvt>
#include <string>

#include "pch.h"
#include "CppUnitTest.h"

#include "../FloorPlanDSL/Evaluator.h"
#include "../FloorPlanDSL/Parser.h"
#include "../FloorPlanDSL/Lexer.h"
#include "../FloorPlanDSL/ParserError.h"
#include "../FloorPlanDSL/Evaluator.h"

#include <variant>


#define EPS 0.00001
#define MSG(msg) [&]{ std::wstringstream _s; _s << (msg); return _s.str(); }().c_str()
#define CAT(A, B)   A##B
#define WSTRING(A)  CAT(L, #A)

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static std::wstring ToString(const AstNodeType type)
{
	std::string narrow_string(AstNodeTypeToString[type]);
	std::wstring wide_string = std::wstring(narrow_string.begin(), narrow_string.end());
	const wchar_t* result = wide_string.c_str();
	return result;
}

static const wchar_t* LOG(const char* str)
{
	std::string narrow_string(str);
	std::wstring wide_string = std::wstring(narrow_string.begin(), narrow_string.end());
	const wchar_t* result = wide_string.c_str();

	return result;
}

namespace UnitTests
{



	TEST_CLASS(UnitTests)
	{
	public:
		void checkParserErrors(Parser& parser)
		{
			std::vector<ParserError*> errors = parser.getErrors();

			if (errors.size() == 0)
			{
				return;
			}

			for (ParserError* err : errors)
			{
				const char* message = err->what();
				Logger::WriteMessage(message);
				Logger::WriteMessage("\n");
			}
			Assert::Fail();
		}
		TEST_METHOD(TestMethod1)
		{
			std::string input =
				"int a = 3;"
				"int b = 5;"
				"measure c = 15cm;"
				"color d = #12333;";

			Lexer lexer;
			lexer.SetInput(input);
			Parser p(lexer);

			ProgramNode* program = p.parseProgram();
			Assert::IsNotNull(program);
			Assert::AreEqual((int)program->statements.size(), 4);

			TokenType expectedVarType[4] = { INT, INT, MEASURE, COLOR };
			std::string expectedVarName[4] = { "a", "b", "c", "d" };
			for (int i : {0, 1, 2, 3})
			{
				std::string expect = expectedVarName[i];
				AstNodeType actual = program->statements[i]->getType();

				Assert::AreEqual(AstNodeType::DeclarationStatement, actual);

				DeclarationStatementNode* declStatement = dynamic_cast<DeclarationStatementNode*>(program->statements[i]);
				IdentifierNode* nameIdent = dynamic_cast<IdentifierNode*>(declStatement->varName);
				ExpressionNode* exprNode = declStatement->value;

				Assert::AreEqual(expectedVarType[i], declStatement->dataType);
				Assert::AreEqual(expectedVarName[i], nameIdent->value);
				Assert::AreEqual(expectedVarName[i], nameIdent->tokenLiteral());
			}
		};



		TEST_METHOD(TestIdentifierExpressionA)
		{
			TestIdentifierExpression("foobar;", "foobar");
		}

		TEST_METHOD(TestIdentifierExpressionB)
		{
			TestIdentifierExpression("a;", "a");
		}

		TEST_METHOD(TestIdentifierExpressionC)
		{
			TestIdentifierExpression("cda;", "cda");
		}

		TEST_METHOD(TestIntegerLiteralA)
		{
			TestIntegerLiteralExpression("10;", 10);
		}

		TEST_METHOD(TestIntegerLiteralB)
		{
			TestIntegerLiteralExpression("5;", 5);
		}

		TEST_METHOD(TestIntegerLiteralC)
		{
			TestIntegerLiteralExpression("123;", 123);
		}


		void TestIdentifierExpression(std::string input, std::string expected)
		{
			// arrange
			Lexer lexer;
			lexer.SetInput(input);
			Parser p(lexer);
			ProgramNode* program = p.parseProgram();

			Assert::AreEqual(1, (int)program->statements.size(), MSG("Invalid statements number"));
			Assert::AreEqual(AstNodeType::ExpressionStatement, program->statements[0]->getType());

			auto stmt = dynamic_cast<ExpressionStatementNode*>(program->statements[0]);

			Assert::AreEqual(AstNodeType::Identifier, stmt->expression->getType());

			auto ident = dynamic_cast<IdentifierNode*>(stmt->expression);

			Assert::AreEqual(expected, ident->value);
			Assert::AreEqual(expected, ident->tokenLiteral());
		}


		void TestIntegerLiteralExpression(std::string input, int expected)
		{
			// arange
			Lexer lexer;
			lexer.SetInput(input);
			Parser p(lexer);
			ProgramNode* program = p.parseProgram();

			Assert::AreEqual(1, (int)program->statements.size(), MSG("INVALID statements number"));
			Assert::AreEqual(AstNodeType::ExpressionStatement, program->statements[0]->getType());

			auto stmt = dynamic_cast<ExpressionStatementNode*>(program->statements[0]);

			Assert::AreEqual(AstNodeType::IntLiteral, stmt->expression->getType());

			auto intNode = dynamic_cast<IntegerLiteralNode*>(stmt->expression);

			Assert::AreEqual(expected, intNode->value);
			Assert::AreEqual(std::to_string(expected), intNode->tokenLiteral());
		}



		TEST_METHOD(TestFloatExpressionA)
		{
			TestFloatExpression("10.3;", 10.3);
		}

		TEST_METHOD(TestFloatExpressionB)
		{
			TestFloatExpression("5.123;", 5.123);
		}

		TEST_METHOD(TestFloatExpressionC)
		{
			TestFloatExpression("123.10;", 123.10);
		}
		void TestFloatExpression(std::string input, float expected)
		{
			// arange
			Lexer lexer;
			lexer.SetInput(input);
			Parser p(lexer);
			ProgramNode* program = p.parseProgram();

			Assert::AreEqual(1, (int)program->statements.size(), MSG("INVALID statements number"));
			Assert::AreEqual(AstNodeType::ExpressionStatement, program->statements[0]->getType());

			auto stmt = dynamic_cast<ExpressionStatementNode*>(program->statements[0]);

			Assert::AreEqual(AstNodeType::FloatLiteral, stmt->expression->getType());

			auto floatNode = dynamic_cast<FloatLiteralNode*>(stmt->expression);

			Assert::IsTrue(abs(expected - floatNode->value) < EPS);
		};

		void TestMeasureExpression(std::string input, float expectedValue, std::string expectedUnit)
		{
			// arange
			Lexer lexer;
			lexer.SetInput(input);
			Parser p(lexer);
			ProgramNode* program = p.parseProgram();

			Assert::AreEqual(1, (int)program->statements.size(), MSG("INVALID statements number"));
			Assert::AreEqual(AstNodeType::ExpressionStatement, program->statements[0]->getType());

			auto stmt = dynamic_cast<ExpressionStatementNode*>(program->statements[0]);

			Assert::AreEqual(AstNodeType::MeasureLiteral, stmt->expression->getType());

			auto measureNode = dynamic_cast<MeasureLiteralNode*>(stmt->expression);

			Assert::AreEqual(expectedUnit, measureNode->unit);
		}
		TEST_METHOD(TestParsingPrefixExpressions)
		{
			struct PrefixTest {
				std::string input;
				std::string op;
				std::variant<int, double> value;
			};

			PrefixTest tests[] = { 
				{"-5;", "-", 5}, 
				{"-15;", "-", 15},
				{"-15.23;", "-", 15.23},
				{"-0.23;", "-", 0.23}
			};

			for (auto test : tests)
			{
				Lexer lexer;
				lexer.SetInput(test.input);
				Parser p(lexer);
				ProgramNode* program = p.parseProgram();

				Assert::AreEqual(1, (int)program->statements.size(), MSG("INVALID statements number"));
				Assert::AreEqual(AstNodeType::ExpressionStatement, program->statements[0]->getType());

				auto stmt = dynamic_cast<ExpressionStatementNode*>(program->statements[0]);

				Assert::AreEqual(AstNodeType::PrefixExpression, stmt->expression->getType());
				PrefixExpressionNode* prefixExpr = dynamic_cast<PrefixExpressionNode*>(stmt->expression);

				Assert::AreEqual(test.op, prefixExpr->op);
				switch (test.value.index())
				{
				case 0: {
					int intValue = std::get<int>(test.value);
					testIntegerLiteral(prefixExpr->right, intValue);

					break;
				}
				case 1: {
					float floatValue = std::get<double>(test.value);
					testFloatLiteral(prefixExpr->right, floatValue);
					break;
				}
				default:
					Assert::Fail();
					break;
				}
			}
		}

		// helpers
		void testIntegerLiteral(ExpressionNode* expr, int value)
		{
			Assert::AreEqual(AstNodeType::IntLiteral, expr->getType());
			IntegerLiteralNode* intNode = dynamic_cast<IntegerLiteralNode*>(expr);

			Assert::AreEqual(value, intNode->value);
			Assert::AreEqual(std::to_string(value), intNode->tokenLiteral());
		}

		void testFloatLiteral(ExpressionNode* expr, double value)
		{
			Assert::AreEqual(AstNodeType::FloatLiteral, expr->getType());
			FloatLiteralNode* floatNode = dynamic_cast<FloatLiteralNode*>(expr);

			Assert::IsTrue(abs(value - floatNode->value) < EPS);
		}



		TEST_METHOD(TestParsingInfixExpression)
		{
			struct InfixTest {
				std::string input;
				std::variant<int, double> left;
				std::string op;
				std::variant<int, double> right;
			};

			InfixTest tests[] = {
				{"3 + 2;", 3, "+", 2},
				{"3 - 2;", 3, "-", 2},
				{"3 / 2;", 3, "/", 2},
				{"3 * 2;", 3, "*", 2}
			};

			for (auto test : tests)
			{
				Lexer lexer;
				lexer.SetInput(test.input);
				Parser p(lexer);
				ProgramNode* program = p.parseProgram();

				Assert::AreEqual(1, (int)program->statements.size(), MSG("INVALID statements number"));

				Assert::AreEqual(AstNodeType::ExpressionStatement, program->statements[0]->getType());
				auto stmt = dynamic_cast<ExpressionStatementNode*>(program->statements[0]);

				Assert::AreEqual(AstNodeType::InfixExpression, stmt->expression->getType());
				InfixExpressionNode* infixExpr = dynamic_cast<InfixExpressionNode*>(stmt->expression);

				Assert::AreEqual(test.op, infixExpr->op);

			}
		}
		// 5 5
		// 5 10
		// 5 15
		// 5 20

		// 10 5
		// 10 10
		// 10 15

		TEST_METHOD(TestOperatorPrecendenceParsing)
		{
			struct OperatorPrecendenceTest {
				std::string input;
				std::string expected;
			};

			//TODO: add more tests
			OperatorPrecendenceTest tests[] = {
				{"-a * b;", "((-a) * b);"},
				{"a * b * c;", "((a * b) * c);"},
				{"a + b + c;", "((a + b) + c);"},
				{"(a + b) / c;" , "((a + b) / c);"},
				{"a + -b / c + c;" , "((a + ((-b) / c)) + c);"},
				{"a + b / c + a * -b;" , "((a + (b / c)) + (a * (-b)));"},
				{"a + b * c * d;", "(a + ((b * c) * d));"}
			};

			for (auto test : tests)
			{
				Lexer lexer;
				lexer.SetInput(test.input);
				Parser p(lexer);
				ProgramNode* program = p.parseProgram();

				std::string actual = program->toString();

				Assert::AreEqual(test.expected, actual);
			}
		}

		TEST_METHOD(TestCallExpression)

		{
			std::string input = "add(1, 2*3, 4+5);";

			Lexer l = Lexer();
			l.SetInput(input);
			Parser p = Parser(l);
			ProgramNode* program = p.parseProgram();
			checkParserErrors(p);

			Assert::AreEqual(1, (int)program->statements.size(), MSG("INVALID statements number"));

			Assert::AreEqual(AstNodeType::ExpressionStatement, program->statements[0]->getType());
			auto stmt = dynamic_cast<ExpressionStatementNode*>(program->statements[0]);

			Assert::AreEqual(AstNodeType::CallExpression, stmt->expression->getType());
			auto callExpr = dynamic_cast < CallExpressionNode*>(stmt->expression);

			Assert::AreEqual(AstNodeType::Identifier, callExpr->function->getType());
			auto funcIdent = dynamic_cast<IdentifierNode*>(callExpr->function);
			Assert::AreEqual(std::string("add"), funcIdent->value);

			Assert::AreEqual(3, (int) callExpr->arguments.size());

			//testLiteralExpression(callExpr->arguments[0], 1);
			//testInfixExpression(callExpr->arguments[1], 2, "*", 3);
			//testInfixExpression(callExpr->arguments[2], 4, "+", 5);
		}

		/*void testLiteralExpression(ExpressionNode* expression, std::string type) {
			if (type == INT_LITERAL) {
				return testIntegerLiteral(expression);
			} else if (type == IDENTIFIER) {
				return testIdentifier();
			}
			else if (type == MEASURE) {
				return testMeasureLiteral();
			}
			else if (type == FLOAT_LITERAL) {
				return testFloatLiteral();
			}
			else {
				Logger::WriteMessage("Type not handled, got: " + type);
				Assert::Fail();
			}
		}*/


		/*TEST_METHOD(TestToString)
		{
			// setup
			std::string input =
				"int a = 3;";
				"int b = 5;"
				"measure c = 15cm;"
				"color d = #12333;";

			Lexer lexer;
			lexer.SetInput(input);
			Parser p(lexer);
			ProgramNode* program = p.parseProgram();

			// act & assert
			Assert::AreEqual(input, program->toString());
		}*/

		TEST_METHOD(TestEvalIntegerExpression)
		{
			struct EvalIntegerTest {
				std::string input;
				int expected;
			};

			EvalIntegerTest tests[] = { 
				{"5;", 5}, 
				{"10;", 10},
				{"-10;", -10 },
				{"-5;", -5},
				{"5 + 5 + 5 + 5 + 5 - 10;", 15},
				{" 2 * 2 * 2;", 8},
				{"-100 + 50 + -50 - 100;", -200}
			};
			for (auto test : tests) {
				auto evaluated = testEval(test.input);
				testIntegerObject(evaluated, test.expected);
			}
		}

		Object* testEval(std::string input) {
			Lexer l = Lexer();
			l.SetInput(input);
			Parser p = Parser(l);
			ProgramNode* program = p.parseProgram();
			Evaluator evaluator(new Enviroment());

			return evaluator.eval(program);
		}

		void testIntegerObject(Object* object, int expected) {
			Assert::IsNotNull(object);
			Assert::AreEqual(INTEGER_OBJ, object->getType());

			auto intObj = dynamic_cast<Integer*>(object);
			Assert::AreEqual(expected, intObj->value);
		}
	};
}
