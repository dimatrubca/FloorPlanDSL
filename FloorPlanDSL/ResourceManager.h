#pragma once
#include <map>
#include <string>

#include "Shader.h"

class ResourceManager {
public:
	static std::map<std::string, Shader> Shaders;

	static void LoadShader(const char* vFile, const char* fFile, std::string name);
	static Shader GetShader(std::string name);
	static void Clear();

private:
	ResourceManager() {};
}