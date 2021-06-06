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

     void drawWall(Wall* wall);
     void drawRoom(Room* room);
     void drawDoor(Door* door);
     void draw(GLuint VAO, int count);
     void drawTriangleStrip(std::vector<float> &vertices, glm::vec3 color); 
     void drawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);

private:
     Shader mainShader;
     Shader spriteShader;

     unsigned int VAO; //TODO: remove
     unsigned int quadVAO;
     void initRenderData();

     float pixelsPerMm = 1;
     int maxWindowWidth;
     int maxWindowHeight;
};

