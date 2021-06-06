#pragma once
#include <vector>
#include "Renderer.h"
#include "MyResourceManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"
#include "GlUtils.h"
#include "../Evaluation/Enviroment.h"
#include "DrawableObjects/GlDrawableObject.h"

class FloorPlan {
public:
	std::vector<GlDrawableObject*> drawableObjects;
	unsigned int width, height;
	FloorPlan(std::string sourceCodePath);
	~FloorPlan();

	// load shaders...
	void init();
	// loop
	void processInput(float dt);
	void update(float dt);
	void render();
	void build();

private:
	std::string sourceCodePath;
	Enviroment* env;
};