#include "TriangleStripDrawable.h"

void TriangleStripDrawable::draw(Renderer* renderer, TextRenderer* textRenderer) {
	renderer->drawTriangleStrip(vertices, color);
}