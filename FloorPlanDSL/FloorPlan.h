#pragma once

class FloorPlan {
public:
	unsigned int width, height;
	FloorPlan(unsigned int width, unsigned int height);
	~FloorPlan();

	// load shaders...
	void init();
	// loop
	void processInput();
	void update(float dt);
	void render();
}