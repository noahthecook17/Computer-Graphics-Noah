#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;

out VS_OUT {
    vec3 normal;
} vs_out;

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

void main(){
	gl_Position = view * model * vec4(aPos, 1.0); 
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
}