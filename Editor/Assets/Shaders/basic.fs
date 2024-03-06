#version 330 core
layout (location=0) out vec4 FragColor;
layout (location=1) out int EntityID;

// from vshader
in vec2 TexCoord;
in vec3 bNormal;
in vec3 FragPos;

// texture samplers
uniform sampler2D texture1;

uniform int ID;

// lighting
// uniform vec3 viewPos;
float AmbientStrength = 0.25;
// float specularStrength = 0.5; :: TODO specular lighting && ambient

struct DirectionalLight
{
	vec3 Direction;
	vec4 Color;
	float Intensity;
};

struct PointLight
{
	vec3 Position;
	vec4 Color;
	float Intensity;

	float Linear;
};

struct SpotLight
{
	vec3 Position;
	vec3 Direction;
	vec4 Color;
	float Intensity;

	float Linear;

	float CutOff;
	float OuterCutOff;
};

float constant = 1;
float quadratic = 1.032f;

uniform DirectionalLight dlight;
uniform PointLight plights[5]; 
uniform SpotLight slights[5]; 


vec4 CalcDirectionalLight(vec3 normal)
{
	vec4 ambient = dlight.Color * AmbientStrength;
	float diffuseFactor = dot(normal, -dlight.Direction);

	vec4 diffuse;
	if (diffuseFactor>0)
	{
		diffuse = dlight.Color * dlight.Intensity * diffuseFactor;
	}
	return (ambient + min(diffuse, 0.7));
}
vec4 CalcPointLight(int Index, vec3 normal)
{
	vec3 norm = normalize(normal);
    vec3 lightDir = normalize(plights[Index].Position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

	// attenuation
    float distance    = length(plights[Index].Position - FragPos);
    float attenuation = 1.0 / (constant + plights[Index].Linear * distance + quadratic * (distance * distance)); 

	return vec4(diff * attenuation) * plights[Index].Color * plights[Index].Intensity;
};
vec4 CalcSpotLight(int Index, vec3 normal)
{
	vec3 norm = normalize(normal);
    vec3 lightDir = normalize(slights[Index].Position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

	// attenuation
    float distance    = length(slights[Index].Position - FragPos);
    float attenuation = 1.0 / (constant + slights[Index].Linear * distance + quadratic * (distance * distance)); 
	float a = slights[Index].Intensity;

	// spotlight (soft edges)
    float theta = dot(lightDir, normalize(-slights[Index].Direction)); 
    float epsilon = (slights[Index].CutOff - slights[Index].OuterCutOff);
    float intensity = clamp((theta - slights[Index].OuterCutOff) / epsilon, 0.0, 1.0);
	
    diff *= intensity;
    diff *= slights[Index].Intensity;
    diff *= attenuation;

	return vec4(slights[Index].Color * diff);
};

void main()
{
	EntityID = ID;

	vec4 BaseColor = texture(texture1, TexCoord);

	vec3 norm = normalize(bNormal);
	// lighting
    vec4 TotalLight = vec4(0.0f);
	TotalLight += max(CalcDirectionalLight(norm), 0);

	for (int i=0; i < 5; i++)
	{
		TotalLight += CalcPointLight(i, norm);
	}

	for (int i=0;i < 5;i++)
	{
		TotalLight += CalcSpotLight(i, norm);
	}

	FragColor = BaseColor * TotalLight;
}
