#version 330 core

layout (location = 0) in vec3 aPos;

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

out vec3 texDir;

void main(){
	mat4 newView = mat4(mat3(view));
	vec4 pos = projection * newView * vec4(aPos, 1.0f);
	gl_Position = pos.xyww;
	texDir = aPos;
};
