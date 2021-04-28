#pragma once
#include <vector>
#include "Renderer.h"
#include "MyResourceManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

class FloorPlan {
public:
	std::vector<DrawableObject*> drawableObjects;
	unsigned int width, height;
	FloorPlan(unsigned int width, unsigned int height);
	~FloorPlan();

	// load shaders...
	void init();
	// loop
	void processInput(float dt);
	void update(float dt);
	void render();
};