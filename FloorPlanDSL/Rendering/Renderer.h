#pragma once
#include <vector>
#include <iostream>
#include "../Parsing/Token.h"
#include "../Objects/Object.h"
#include "../Objects/Room.h"
#include "../Objects/Door.h"
#include "../Objects/Wall.h"
#include "Shader.h"
#include "GlUtils.h"
#include "Texture2D.h"



class Renderer
{
public:
     Renderer(Shader& shader, Shader& spriteShader);
     ~Renderer();

     void draw(GLuint VAO, int count);
     void drawTriangleStrip(std::vector<float> &vertices, glm::vec3 color); 
     void drawLineStrip(std::vector<float>& vertices, glm::vec3 color);
     void drawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);

     void setWindowDimensions(int requestedWidth, int requestedHeight, int& width, int& height);

private:
     Shader mainShader;
     Shader spriteShader;

     unsigned int VAO; //TODO: remove
     unsigned int quadVAO;
     void initRenderData();
     void normalizeVertices(std::vector<float>& vertices);
     void normalizeVertices(glm::vec2& position);

     float unitsPerPixel = 1;
     int maxWindowWidth = 500;
     int maxWindowHeight = 500;
};

