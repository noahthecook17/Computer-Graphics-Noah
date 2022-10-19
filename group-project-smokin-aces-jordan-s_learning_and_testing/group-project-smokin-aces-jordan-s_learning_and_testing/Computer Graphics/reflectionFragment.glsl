#version 330 core

in vec3 Normal;
in vec3 Pos;

uniform samplerCube cubeTex;
uniform vec3 cameraPos;

out vec4 FragColor;

void main(){

	vec3 I = normalize(Pos.xyz - cameraPos);
	vec3 R = reflect(I, normalize(Normal));

	if(!gl_FrontFacing){
		FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);	
	}	
	else
		FragColor = vec4(texture(cubeTex, R).rgb, 1.0f);	
};