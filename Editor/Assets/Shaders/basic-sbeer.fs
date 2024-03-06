#version 330 core
layout (location=0) out vec4 FragColor;
layout (location=1) out int EntityID;

in vec2 TexCoord;
in vec3 bNormal;
in vec3 FragPos;

// texture samplers
uniform sampler2D texture1;

uniform int ID;

// lighting
uniform vec4 LightColor;
uniform float LightIntensity;
uniform vec3 lightPos;
uniform vec3 viewPos;

float AmbientStrength = 0.25;
float specularStrength = 0.5;

void main()
{
	EntityID = ID;
	// lighting:
    vec4 tex = texture(texture1, TexCoord);

	// ambient
		vec4 ambient = LightColor * AmbientStrength;

	// difuse:
		vec3 norm = normalize(bNormal);
		vec3 lightDir = normalize(lightPos - FragPos); 

		float diff = max(dot(norm, lightDir), 0.0);
		vec4 diffuse = diff * LightColor * LightIntensity;

	// specular:
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec4 specular = specularStrength * spec * LightColor;  

	// FragColor = vec4(norm*lightDir, 1.0f) * tex;
	// FragColor = vec4(lightDir, 1.0f);
	FragColor = (specular + ambient + diffuse) * tex;
}