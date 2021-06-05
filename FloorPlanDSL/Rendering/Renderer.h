#pragma once
#include <iostream>
#include "../Parsing/Token.h"
#include "../Objects/Object.h"
#include "../Objects/Wall.h"
#include "Shader.h"
#include "GlUtils.h"

class Renderer
{
public:
     Renderer(Shader& shader);
     ~Renderer();

     void drawWall(Wall wall);
     void draw(GLuint VAO, int count);

private:
     Shader shader;
     unsigned int VAO;
     void initRenderData();
};