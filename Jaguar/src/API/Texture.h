#pragma once

#include "Shader.h"
#include "Core/DEFINES.h"

class JAGUAR_API Texture
{
public:
	Texture(const char* path);
	~Texture();

	void Bind();
	void UnBind();

	inline unsigned int GetImage() { return m_ID; };

private:
	unsigned int m_ID;
	// Ref<Shader> m_shader;
};
