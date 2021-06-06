#pragma once

#include "stb_image.h"

#include "MyResourceManager.h"

std::map<std::string, Texture2D> MyResourceManager::Textures;
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



Texture2D MyResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D MyResourceManager::GetTexture(std::string name)
{
	return Textures[name];

}

Texture2D MyResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
	// create texture object
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	// now generate texture
	texture.Generate(width, height, data);
	// and finally free image data
	stbi_image_free(data);
	return texture;
}
