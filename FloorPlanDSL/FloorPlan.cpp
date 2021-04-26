#pragma once

#include "FloorPlan.h"
#include "Renderer.h"
#include "ResourceManager.h"

Renderer* renderer;

FloorPlan::FloorPlan(unsigned int width, unsigned int height) : width(width), height(height) {};
FloorPlan::~FloorPlan()
{
	delete renderer;
}

void FloorPlan::init() {
	ResourceManager::LoadShader("res/shaders/vertex.shader", "res/shaders/fragment.shader", nullptr, "rect");

	glm::mat4 projection = glm::ortho(0.0f, this->width, this->height
}