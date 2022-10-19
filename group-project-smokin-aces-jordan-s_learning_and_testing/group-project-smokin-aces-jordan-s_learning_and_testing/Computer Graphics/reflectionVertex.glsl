#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 normal;

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

out vec3 Normal;
out vec3 Pos;

void main(){
	vec4 pos = model * vec4(aPos, 1.0f);

	Normal = normalize(normal);
	Pos = vec3(pos);
	
	gl_Position = projection * view * pos;
};