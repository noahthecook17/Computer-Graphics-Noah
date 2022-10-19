#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoords;

uniform mat4 model;

layout (std140) uniform Matrices
{
						// base alignment	// aligned offset
	mat4 view;			// 16				// 0
						// 16				// 16
						// 16				// 32
						// 16				// 48
	mat4 projection;	// 16				// 64
						// 16				// 80
						// 16				// 96
						// 16				// 112
};

out vec2 TexCoords;


void main()
{    
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    
    TexCoords = texCoords;
};