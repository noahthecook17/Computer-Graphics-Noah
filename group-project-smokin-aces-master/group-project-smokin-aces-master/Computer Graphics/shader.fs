#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 ourColor;

uniform sampler2D texture_wood;

void main()
{
  FragColor = texture(texture_wood, TexCoord);
};
