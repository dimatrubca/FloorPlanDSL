#pragma once
#include <vector>
#include "../../Objects/Object.h"
#include "../Renderer.h"
#include "../TextRenderer.h"

class GlDrawableObject
{
public:
     GlDrawableObject() {};
     virtual void draw(Renderer* renderer, TextRenderer* textRenderer) = 0;
};

