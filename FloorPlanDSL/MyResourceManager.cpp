#pragma once

#include "MyResourceManager.h"

//std::map<std::string, Texture2D> MyResourceManager::Textures;
std::map<std::string, Shader> MyResourceManager::Shaders;

Shader MyResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

void MyResourceManager::LoadShader(const char* vFile, const char* fFile, std::string name)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ensure ifstream objets can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// open files
		vShaderFile.open(vFile);
		fShaderFile.open(fFile);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "[ERROR]::SHADER::FILE_NOT_READ_SUCCESSFULLY " << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();



	Shader shader;
	shader.compile(vShaderCode, fShaderCode);

	Shaders[name] = shader;
}

