#version 330 core

in vec2 TexCoords;
uniform sampler2D texture1;

out vec4 FragColor;

void main() {
	
	vec4 texColor = texture(texture1, TexCoords);

	if(texColor.a < 0.1)
		discard;

	FragColor = texColor;
	//FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}