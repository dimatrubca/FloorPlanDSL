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
#include "Evaluator.h"
#include "Lexer.h"
#include "Parser.h"


GLenum glCheckError_(const char* file, int line)
{
     GLenum errorCode;
     while ((errorCode = glGetError()) != GL_NO_ERROR)
     {
          std::string error;
          switch (errorCode)
          {
          case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
          case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
          case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
          case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
          case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
          case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
          case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
          }
          std::cout << error << " | " << file << " (" << line << ")" << std::endl;
          exit(0);
     }
     return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

FloorPlan floorPlan(SCREEN_WIDTH, SCREEN_HEIGHT);

void printParserErrors2(std::vector<ParserError*> const& errors) {
     for (auto err : errors) {
          std::cout << err->what() << "\n";
     }
}

void REPL() {
     Evaluator evaluator(new Enviroment());

     std::ifstream file("res/sample.txt");
     std::string input((std::istreambuf_iterator<char>(file)),
          std::istreambuf_iterator<char>());

     Lexer lexer;
     lexer.SetInput(input);
     Parser parser(lexer);
     ProgramNode* program = parser.parseProgram();

     if (parser.getErrors().size() != 0) {
          printParserErrors2(parser.getErrors());
          return;
     }

     Object* evalResult = evaluator.eval(program);
     if (evalResult) {
          std::cout << evalResult->toString();
          std::cout << '\n';
     }

     Object* wallOb = evaluator.env->get("abd");
     Object* roomOb = evaluator.env->get("room1");
     if (Wall* wall = dynamic_cast<Wall*>(wallOb)) {
          floorPlan.drawableObjects.push_back(wall);
     }
     else {
          std::cout << "Error: no wall with id abd";
     }

     if (Room* room = dynamic_cast<Room*>(roomOb)) {
          floorPlan.drawableObjects.push_back(room);
     }
     else {
          std::cout << "Error: no room with id room1";
     }
}

int main() {
	if (!glfwInit())
          return -1;

     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

     // create window
     GLFWwindow* window = glfwCreateWindow(800, 600, "Floor Plan", NULL, NULL);
     if (window == NULL) {
          std::cout << "Failed to create GLFW window" << std::endl;
          glfwTerminate();
          return -1;
     }

     glfwMakeContextCurrent(window);
     glewExperimental = true; // Needed in core profile
     if (glewInit() != GLEW_OK) {
          fprintf(stderr, "Failed to initialize GLEW\n");
          return -1;
     }

     glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
     glfwMakeContextCurrent(window);
     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

     // tell GLFW to capture our mouse
     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
     glEnable(GL_DEPTH_TEST);

     glCheckError();

     floorPlan.init();
     glCheckError();

     // deltaTime variables
     float deltaTime = 0.0f;
     float lastFrame = 0.0f;
     glCheckError();
     REPL();
     glCheckError();
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
          floorPlan.processInput(deltaTime);
          glCheckError();

          // update game state
          // -----------------
          floorPlan.update(deltaTime);
          glCheckError();

          // render
          // ------
          glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          floorPlan.render();

          glfwSwapBuffers(window);
          glfwPollEvents();
          glCheckError();
     }

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
     // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
          glfwSetWindowShouldClose(window, true);
     /*if (key >= 0 && key < 1024)
     {
          if (action == GLFW_PRESS)
               floorPlan.Keys[key] = true;
          else if (action == GLFW_RELEASE)
               floorPlan.Keys[key] = false;
     }*/
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
     // make sure the viewport matches the new window dimensions; note that width and 
     // height will be significantly larger than specified on retina displays.
     glViewport(0, 0, width, height);
}