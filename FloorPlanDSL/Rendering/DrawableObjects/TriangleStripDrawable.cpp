#include "TriangleStripDrawable.h"

void TriangleStripDrawable::draw(Renderer* renderer) {
	renderer->drawTriangleStrip(vertices, color);
}