#version 330 core

struct Light {
	vec3 position;

	float quadratic;
	float linear;
	float constant;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 TexCoord;
in vec4 FragPos;
in vec4 Normal;
in vec4 lightPosition;

out vec4 FragColor;

uniform Light light;
uniform vec4 ourColor;
uniform sampler2D texture;
uniform vec3 viewPos;


void main()
{		
	vec4 normal = normalize(Normal);	

	vec3 ambient = light.ambient;
	
	vec4 lightDir = normalize(lightPosition - FragPos);

	vec4 viewDir = normalize(vec4(viewPos, 1.0f) - FragPos);

	vec4 halfwayDir = normalize(lightDir + viewDir);

	float blinnPhong = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);

	float distance = length(lightPosition - FragPos);
	
	float cosTheta = max(dot(normal, lightDir), 0.0f);

	float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance)); 

	vec3 specular = light.specular * blinnPhong;

	vec3 diffuse = cosTheta * attenuation * light.diffuse;

	if(ourColor.x != 1.0f){
		FragColor = texture(texture, TexCoord) * vec4(diffuse + ambient + specular,1.0f); 
		//FragColor = vec4(0.5f, vec2(gl_FragCoord.z), 1.0f);
	}
	else FragColor = ourColor;
	
};
