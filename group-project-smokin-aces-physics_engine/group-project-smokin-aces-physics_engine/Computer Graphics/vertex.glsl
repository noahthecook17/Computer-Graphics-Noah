#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

out vec3 ourColor;
out vec2 TexCoord;
out vec4 lightPosition;
out vec4 Normal;
out vec4 FragPos;

void main()
{    
    
    gl_Position =  projection * view * model * vec4(aPos, 1.0f);

    FragPos = view * model * vec4(aPos, 1.0f);

    lightPosition = view * vec4(lightPos, 1.0f);
    
    ourColor = aColor;
    
    Normal = view * model * vec4(aNormal, 0.0f);

    TexCoord = aTexCoord;
};
