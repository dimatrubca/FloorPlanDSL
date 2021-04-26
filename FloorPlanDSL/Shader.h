#pragma once
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader {
public:
	unsigned int ID;
	Shader() {};
	// user/activate the shader
	void use();
	void compile(const char* vertexSrc, const char* fragmentSrc);
	// utility uniform functions
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 matrix) const;

private:
	void checkCompileErrors();
};