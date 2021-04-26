#pragma once
#include <iostream>
#include "Token.h"
#include "Object.h"
#include "Shader.h"
#include "Object.h"

class Renderer
{
public:
     Renderer(Shader& shader);
     ~Renderer();

     void drawWall(Wall wall);

private:
     Shader shader;
     unsigned int VAO;
     void initRenderData();
};