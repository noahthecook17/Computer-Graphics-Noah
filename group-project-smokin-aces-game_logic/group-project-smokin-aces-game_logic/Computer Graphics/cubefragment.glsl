#version 330 core

in vec3 texDir;

uniform samplerCube cubeTex;

out vec4 FragColor;

void main(){
	FragColor = texture(cubeTex, texDir);	
	
};