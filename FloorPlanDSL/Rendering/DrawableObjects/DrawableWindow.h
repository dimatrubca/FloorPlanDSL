#pragma once
#include "TriangleStripDrawable.h"
#include "../../Objects/Window.h"
#include "../TextRenderer.h"

class DrawableWindow : public TriangleStripDrawable {
public:
	DrawableWindow(Window* window);
	virtual void draw(Renderer* renderer, TextRenderer* textRenderer);
private:
	Window* window;
	std::vector<float> lineVertices;
};

