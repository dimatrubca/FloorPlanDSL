#pragma once

#include "FloorPlan.h"

Renderer* renderer;

FloorPlan::FloorPlan(unsigned int width, unsigned int height) : width(width), height(height) {};
FloorPlan::~FloorPlan()
{
	//delete renderer;
}


void FloorPlan::init() {
	MyResourceManager::LoadShader("res/shaders/vertex.shader", "res/shaders/fragment.shader", "mainShader");
	glCheckError0();

	glm::mat4 projection = glm::ortho(0.0f, 
		(float)this->width, 0.0f, (float) this->height,-10.0f, 10.0f);
		
	MyResourceManager::GetShader("mainShader").use();
	glCheckError0();

	MyResourceManager::GetShader("mainShader").setMat4("projection", projection);
	glCheckError0();

	// set render specific controls
	Shader shader = MyResourceManager::GetShader("mainShader");
	renderer = new Renderer(shader);
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