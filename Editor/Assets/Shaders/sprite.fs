#version 330 core
layout (location=0) out vec4 FragColor;
layout (location=1) out int EntityID;

in vec2 TexCoord;

uniform vec4 color;
uniform int ID;


// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = color*texture(texture1, TexCoord);
	// FragColor = color;
	// FragColor = vec4(1, 1, 1, 1);
	EntityID = ID;
}