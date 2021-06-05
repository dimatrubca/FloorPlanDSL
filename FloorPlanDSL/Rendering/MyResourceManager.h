#pragma once
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"

class MyResourceManager {
public:
	static std::map<std::string, Shader> Shaders;
	//static std::map<std::string, Texture2D> Textures;

	static void LoadShader(const char* vFile, const char* fFile, std::string name);
	static Shader GetShader(std::string name);
	static void Clear();

private:
	MyResourceManager() {};
};