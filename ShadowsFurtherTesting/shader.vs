#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{   
    
    //if(aPos.y > 5.0f){
    //   model = mat4(vec4(), vec4(), vec4(), vec4());
    //}
    gl_Position =  projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    ourColor = aColor;

    TexCoord = aTexCoord;
};
