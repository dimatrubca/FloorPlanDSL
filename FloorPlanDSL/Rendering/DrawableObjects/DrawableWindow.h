#pragma once
#include "TriangleStripDrawable.h"
#include "../../Objects/Window.h"

class DrawableWindow : public TriangleStripDrawable {
public:
	DrawableWindow(Window* window);
	virtual void draw(Renderer* renderer);
private:
	Window* window;
	std::vector<float> lineVertices;
};

