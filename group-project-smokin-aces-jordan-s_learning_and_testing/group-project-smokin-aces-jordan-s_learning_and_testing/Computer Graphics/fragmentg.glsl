#version 330 core

out vec4 FragColor;

float near = 0.1; 
float far  = 100.0; 

uniform float time;
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main(){
	//float depth = gl_FragCoord.z/
	if(time > 5.0)
		FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	else
		FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z)), 1.0f);	
}