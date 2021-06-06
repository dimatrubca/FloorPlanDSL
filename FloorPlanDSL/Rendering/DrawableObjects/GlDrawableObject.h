#pragma once
#include <vector>
#include "../../Objects/Object.h"
#include "../Renderer.h"

class GlDrawableObject
{
public:
     GlDrawableObject() {};
     virtual void draw(Renderer* renderer) = 0;
};

