#pragma once
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"
#include "Texture2D.h"

class MyResourceManager {
public:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;

	static void LoadShader(const char* vFile, const char* fFile, std::string name);
	static Shader GetShader(std::string name);
	// loads (and generates) a texture from file
	static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
	// retrieves a stored texture
	static Texture2D GetTexture(std::string name);
	// properly de-allocates all loaded resources
	//static void Clear();

private:
	static Texture2D loadTextureFromFile(const char* file, bool alpha);

	MyResourceManager() {};
};