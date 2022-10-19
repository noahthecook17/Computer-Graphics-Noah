#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoords;

uniform mat4 model;

out vec2 TexCoords;


void main()
{
	gl_Position = model * vec4(aPos, 1.0f);
	TexCoords = texCoords;
};