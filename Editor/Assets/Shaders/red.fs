#version 330 core
layout (location=0) out vec4 FragColor;
layout (location=1) out int EntityID;

in vec2 TexCoord;
// layout (location=1) int int EntityIDIn;

// texture samplers
uniform sampler2D texture1;
// uniform sampler2D texture2;

uniform int ID;

void main()
{
	// FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	// FragColor = vec4(TexCoord, 0.5, 1);
	// FragColor = vec4(TexCoord, 0, 1);
	FragColor = vec4(1.0f, 0.0f, 0.0f, 1.f);
	EntityID = ID;
}