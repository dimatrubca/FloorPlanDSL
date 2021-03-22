#pragma once
#include "Object.h"
#include "Enviroment.h"
#include "AST.h"

class Evaluator {
public:
	Evaluator(Enviroment* env);
	Object* eval(Node* node);

private:
	Enviroment* env;
};