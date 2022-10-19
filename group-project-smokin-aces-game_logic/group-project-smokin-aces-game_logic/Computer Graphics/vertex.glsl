#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

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

uniform vec3 lightPos;

out vec3 ourColor;
out vec2 TexCoord;
out vec4 lightPosition;
out vec4 Normal;
out vec4 FragPos;

void main()
{      
    vec4 position =  projection * view * model * vec4(aPos, 1.0f);

    gl_PointSize = position.z;

    gl_Position = position;

    FragPos = view * model * vec4(aPos, 1.0f);

    lightPosition = view * vec4(lightPos, 1.0f);
    
    ourColor = aColor;
    
    Normal = view * model * vec4(aNormal, 0.0f);

    TexCoord = aTexCoord;
};
