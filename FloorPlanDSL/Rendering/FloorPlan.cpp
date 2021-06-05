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

Renderer* renderer;

FloorPlan::FloorPlan(std::string sourceCodePath) : sourceCodePath(sourceCodePath) {};


FloorPlan::~FloorPlan()
{
	//delete renderer;
}


void FloorPlan::init() {
     // parse program

     //glewExperimental = true; // Needed in core profile
     //if (glewInit() != GLEW_OK) {
     //     fprintf(stderr, "Failed to initialize GLEW (\n");
     //     return;
     //}

     std::ifstream file(sourceCodePath);
     std::string input((std::istreambuf_iterator<char>(file)),
          std::istreambuf_iterator<char>());

     Lexer lexer;
     lexer.SetInput(input);

     Parser parser(lexer);
     this->program = parser.parseProgram();

     if (parser.getErrors().size() != 0) {
          std::cout << "parse errors, todo: print them";
          return;
     }

     //...
}

void FloorPlan::build() {

     if (!glfwInit())
          return;

     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

     // create window
     GLFWwindow* window = glfwCreateWindow(800, 600, "Floor Plan", NULL, NULL);
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

     glViewport(0, 0, 800, 600);
     glfwMakeContextCurrent(window);

     // tell GLFW to capture our mouse
     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
     glEnable(GL_DEPTH_TEST);

     //glCheckError();

     /* from init */
     Evaluator evaluator(new Enviroment());

     Object* evalResult = evaluator.eval(program);
     this->width = evaluator.env->windowWidth;
     this->height = evaluator.env->windowHeight;

     std::cout << width << " x " << height << '\n';

     if (evalResult) {
          std::cout << evalResult->toString();
          std::cout << '\n';
     }

     Object* wallOb = evaluator.env->get("abd");
     Object* roomOb = evaluator.env->get("room1");

     auto drawables = evaluator.env->getDrawableObjects();

     this->drawableObjects.insert(drawableObjects.end(), drawables.begin(), drawables.end()); //
     /*
     if (Wall* wall = dynamic_cast<Wall*>(wallOb)) {
          this->drawableObjects.push_back(wall);
     }
     else {
          std::cout << "Error: no wall with id abd";
     }

     if (Room* room = dynamic_cast<Room*>(roomOb)) {
          this->drawableObjects.push_back(room);
     }
     else {
          std::cout << "Error: no room with id room1";
     }
     */

     /* */

     glfwSetWindowSize(window, this->width, this->height);
     glViewport(0, 0, this->width, this->height);

     /* */

     // draw all

     MyResourceManager::LoadShader("res/shaders/vertex.shader", "res/shaders/fragment.shader", "mainShader");
     glCheckError0();

     glm::mat4 projection = glm::ortho(0.0f,
          (float)this->width, 0.0f, (float)this->height, -10.0f, 10.0f);

     MyResourceManager::GetShader("mainShader").use();
     glCheckError0();

     MyResourceManager::GetShader("mainShader").setMat4("projection", projection);
     glCheckError0();

     // set render specific controls
     Shader shader = MyResourceManager::GetShader("mainShader");
     renderer = new Renderer(shader);
     /**************/


     // deltaTime variables
     float deltaTime = 0.0f;
     float lastFrame = 0.0f;
     //glCheckError();
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
     }

}

void FloorPlan::render()
{
	for (auto drawable : drawableObjects) {
		drawable->draw(*renderer);
	}
}

void FloorPlan::processInput(float dt)
{
}

void FloorPlan::update(float dt)
{
}