#pragma once

#include <iostream>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

#include "FloorPlan.h"
#include "../Evaluation/Evaluator.h"
#include "../Parsing/Lexer.h"
#include "../Parsing/Parser.h"
#include "DrawableObjects/DrawableWall.h"
#include "DrawableObjects/DrawableRoom.h"
#include "DrawableObjects/DrawableBed.h"
#include "DrawableObjects/DrawableDoor.h"
#include "DrawableObjects/DrawableWindow.h"
#include "DrawableObjects/DrawableChair.h"

#include <chrono>
#include <thread>
#include "DrawableObjects/DrawableTable.h"
#include "TextRenderer.h"

Renderer* renderer;
TextRenderer* textRenderer;

FloorPlan::FloorPlan(std::string sourceCodePath) : sourceCodePath(sourceCodePath) {};


FloorPlan::~FloorPlan()
{
	//destroy renderer;
}


void FloorPlan::init() {
     std::ifstream file(sourceCodePath);
     std::string input((std::istreambuf_iterator<char>(file)),
          std::istreambuf_iterator<char>());

     Lexer lexer;
     lexer.SetInput(input);

     Parser parser(lexer);
     ProgramNode* program = parser.parseProgram();

     if (parser.getErrors().size() != 0) {
          std::cout << "parse errors";
          printParserErrors(parser.getErrors());
          exit(0);
     }

     Evaluator evaluator(new Enviroment());

     Object* evalResult = evaluator.eval(program);
     this->widthUnits = evaluator.env->windowWidth;
     this->heightUnits = evaluator.env->windowHeight;

     std::cout << widthUnits << " x " << heightUnits << '\n';

     if (evalResult) {
          std::cout << evalResult->toString();
          std::cout << '\n';
          
          this->env = evaluator.env;

          for (auto wall : evaluator.env->walls) {
               DrawableWall* drawableWall = new DrawableWall(wall);
               this->drawableObjects.push_back((GlDrawableObject*) drawableWall);
          }
          
          for (auto room : evaluator.env->rooms) {
               DrawableRoom* drawableRoom = new DrawableRoom(room);
               this->drawableObjects.push_back((GlDrawableObject*)drawableRoom);
          }

          for (auto bed : evaluator.env->beds) {
               DrawableBed* drawableBed = new DrawableBed(bed);
               this->drawableObjects.push_back((GlDrawableObject*)drawableBed);
          }

          for (auto door : evaluator.env->doors) {
               DrawableDoor* drawableDoor = new DrawableDoor(door);
               this->drawableObjects.push_back((GlDrawableObject*)drawableDoor);
          }

          for (auto window : evaluator.env->windows) {
               DrawableWindow* drawableWindow = new DrawableWindow(window);
               this->drawableObjects.push_back((GlDrawableObject*)drawableWindow);
          }

          for (auto chair : evaluator.env->chairs) {
               DrawableChair* drawableChair = new DrawableChair(chair);
               this->drawableObjects.push_back((GlDrawableObject*)drawableChair);
          }

          for (auto table : evaluator.env->tables) {
               DrawableTable* drawableTable = new DrawableTable(table);
               this->drawableObjects.push_back((GlDrawableObject*)drawableTable);
          }
     }

     setDimensions();


}

//TODO: rename to render
void FloorPlan::build() {
     if (!glfwInit())
          return;

     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

     // create window
     GLFWwindow* window = glfwCreateWindow(this->width, this->height, "Floor Plan", NULL, NULL);
     if (window == NULL) {
          std::cout << "Failed to create GLFW window" << std::endl;
          glfwTerminate();
          return;
     }

     glfwMakeContextCurrent(window);
     glewExperimental = true; // Needed in core profile
     if (glewInit() != GLEW_OK) {
          fprintf(stderr, "Failed to initialize GLEW\n");
          return;
     }

     glViewport(0, 0, this->width, this->height);
     glfwMakeContextCurrent(window);

     // tell GLFW to capture our mouse
     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
     glEnable(GL_DEPTH_TEST);

     //glCheckError();

     textRenderer = new TextRenderer(this->width, this->height);
     textRenderer->Load("res/fonts/arial.ttf", 12);
     textRenderer->unitsPerPixel = this->unitsPerPixel;
     textRenderer->width = this->width;
     textRenderer->height = this->height;

     // draw all
     glCheckError0();
     MyResourceManager::LoadShader("res/shaders/vertex.shader", "res/shaders/fragment.shader", "mainShader");
     glCheckError0();
     MyResourceManager::LoadShader("res/shaders/spriteVertex.shader", "res/shaders/spriteFragment.shader", "spriteShader");
     glCheckError0();
     MyResourceManager::LoadTexture("res/textures/bed.png", true, "bed");
     glCheckError0();
     MyResourceManager::LoadTexture("res/textures/table.png", true, "table");
     glCheckError0();
     MyResourceManager::LoadTexture("res/textures/chair.png", true, "chair");
     glCheckError0();

     glm::mat4 projection = glm::ortho(0.0f,
          (float)this->width, 0.0f, (float)this->height, -10.0f, 10.0f);

     MyResourceManager::GetShader("mainShader").use();
     glCheckError0();

     MyResourceManager::GetShader("mainShader").setMat4("projection", projection);
     glCheckError0();

     MyResourceManager::GetShader("spriteShader").use();
     MyResourceManager::GetShader("spriteShader").setMat4("projection", projection);

     // set render specific controls
     Shader mainShader = MyResourceManager::GetShader("mainShader");
     Shader spriteShader = MyResourceManager::GetShader("spriteShader");
     renderer = new Renderer(mainShader, spriteShader);
     renderer->unitsPerPixel = this->unitsPerPixel;
     /**************/


     // deltaTime variables
     float deltaTime = 0.0f;
     float lastFrame = 0.0f;
     //glCheckError();
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     while (!glfwWindowShouldClose(window))
     {
          // calculate delta time
          // --------------------
          float currentFrame = glfwGetTime();
          deltaTime = currentFrame - lastFrame;
          lastFrame = currentFrame;
          glfwPollEvents();

          // manage user input
          // -----------------
          processInput(deltaTime);
          //glCheckError();

          // update game state
          // -----------------
          this->update(deltaTime);
          //glCheckError();

          // render
          // ------
          glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          this->render();

          glfwSwapBuffers(window);
          glfwPollEvents();
          //glCheckError();

          std::this_thread::sleep_for(std::chrono::milliseconds(3000));
     }

}

void FloorPlan::render()
{
     for (auto drawable : drawableObjects) {
          drawable->draw(renderer, textRenderer);
     }
     textRenderer->RenderText("Lives:", 5.0f, 5.0f, 1.0f);
}

void FloorPlan::processInput(float dt)
{
}

void FloorPlan::update(float dt)
{
}

void FloorPlan::setDimensions() {
     if (widthUnits <= maxWidthPixels && heightUnits <= maxHeightPixels) {
          width = widthUnits;
          height = heightUnits;
          unitsPerPixel = widthUnits / width;
     }

     if (heightUnits * 1.0 / widthUnits * maxWidthPixels <= maxHeightPixels) {
          width = maxWidthPixels;
          height = heightUnits * 1.0 / widthUnits * width;
          unitsPerPixel = widthUnits / width;
     }
     else {
          height = maxHeightPixels;
          width = widthUnits * 1.0 / heightUnits * height;
          unitsPerPixel = heightUnits / height;
     }

};