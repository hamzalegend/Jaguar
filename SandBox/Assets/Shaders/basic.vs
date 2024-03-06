#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 bNormal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoord = aTexCoord;
	bNormal = aNormals;
	bNormal = mat3(transpose(inverse(model))) * aNormals; 
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}