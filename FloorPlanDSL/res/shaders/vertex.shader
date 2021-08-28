#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 projection;
uniform vec3 u_color;

void main()
{
	gl_Position = projection * vec4(position, 1.0);
}