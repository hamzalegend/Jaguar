#pragma once

#include <vector>
#include "Core/DEFINES.h"

class JAGUAR_API VertexBuffer
{
public:
	VertexBuffer(std::vector<float> data);
	~VertexBuffer();

	void Bind();
	void UnBind();

private:
	unsigned int m_ID;

};

class JAGUAR_API IndexBuffer
{
public:
	IndexBuffer(std::vector<unsigned int> data);
	~IndexBuffer();

	void Bind();
	void UnBind();

private:
	unsigned int m_ID;

};

class JAGUAR_API VertexArray
{
public:
	VertexArray();
	~VertexArray();

	inline int GetPerVertexCounr() {
		int total = 0;
		for (int i = 0; i < m_layout.size(); i++)
			total += m_layout[i];
		return total;
	}

	void Bind();
	void UnBind();
	// size is the number of floats, so for eg. vec2s have a size of 2;
	void Addlayout(int size);
	void Flush();
private:

	unsigned int m_ID;
	unsigned int m_stride = 0;
	std::vector<int> m_layout;
};
