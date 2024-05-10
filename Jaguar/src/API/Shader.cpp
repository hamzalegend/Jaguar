#include "Shader.h"

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>


Shader::Shader(const char* vpath, const char* fpath)
{
	std::string vertSrc;
	std::string fragSrc;

	{
		std::string line;
		std::ifstream myfile(vpath);
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				vertSrc += line + "\n";
			}
			myfile.close();
		}

		else std::cout << "[ERROR]: Unable to open shader file '" + std::string(vpath) + "'\n";
	}

	{
		std::string line;
		std::ifstream myfile(fpath);
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				fragSrc += line + "\n";
			}
			myfile.close();
		}

		else std::cout << "[ERROR]: Unable to open shader file '" + std::string(fpath) + "'\n";
		// std::cout << std::filesystem::current_path() << "'\n";
	}

	const char* vsrc = vertSrc.c_str();
	const char* fsrc = fragSrc.c_str();

	// std::cout << vertSrc.c_str();
	// std::cout << fragSrc.c_str();

	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vsrc, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fsrc, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);
	glLinkProgram(m_ID);
	// check for linking errors
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

void Shader::Bind()
{
	glUseProgram(m_ID);
}

void Shader::UnBind()
{
	glUseProgram(m_ID);
}

void Shader::SetInt(const char* name, int v)
{
	if (glGetUniformLocation(m_ID, name) == -1) std::cout << "Cannot Find Uniform With Name: " << name << "\n";
	glUniform1i(glGetUniformLocation(m_ID, name), v);
	// std::cout << "seting : " << name << " to " << v << "\n";
}

void Shader::SetFloat(const char* name, float v)
{
	if (glGetUniformLocation(m_ID, name) == -1) std::cout << "Cannot Find Uniform With Name: " << name << "\n";
	glUniform1f(glGetUniformLocation(m_ID, name), v);
}

void Shader::SetVec3(const char* name, glm::vec3 v)
{
	if (glGetUniformLocation(m_ID, name) == -1) std::cout << "Cannot Find Uniform With Name: " << name << "\n";
	glUniform3f(glGetUniformLocation(m_ID, name), v.x, v.y, v.z);
}

void Shader::SetVec4(const char* name, glm::vec4 v)
{
	if (glGetUniformLocation(m_ID, name) == -1) std::cout << "Cannot Find Uniform With Name: " << name << "\n";
	glUniform4f(glGetUniformLocation(m_ID, name), v.x, v.y, v.z, v.w);
}

void Shader::SetMat4(const char* name, glm::mat4 v)
{
	if (glGetUniformLocation(m_ID, name) == -1) std::cout << "Cannot Find Uniform With Name: " << name << "\n";
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, glm::value_ptr(v));
}
