#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
public:
	Shader(const char* vpath, const char* fpath);
	~Shader();

	void Bind();
	void UnBind();

	void SetInt(const char* name, int v);
	void SetFloat(const char* name, float v);
	void SetVec3(const char* name, glm::vec3);
	void SetVec4(const char* name, glm::vec4);
	void SetMat4(const char* name, glm::mat4 v);

private:
	unsigned int m_ID;

};


