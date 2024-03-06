#include "Buffer.h"

#include <glad/glad.h>

#include<iostream>

VertexBuffer::VertexBuffer(std::vector<float> data)
{
	glGenBuffers(1, &m_ID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*data.size(), &data[0], GL_STATIC_DRAW);
}


VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//==============================

IndexBuffer::IndexBuffer(std::vector<unsigned int> data)
{
	glGenBuffers(1, &m_ID);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * data.size(), &data[0], GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::UnBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ID);
	Bind();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_ID);
}

void VertexArray::UnBind()
{
	glBindVertexArray(0);
}

void VertexArray::Addlayout(int size)
{
	m_layout.push_back(size);
	m_stride += size;
}

void VertexArray::Flush()
{
	Bind();

	int sum = 0;
	for (int i = 0; i < m_layout.size(); i++)
	{
		glVertexAttribPointer(i, m_layout[i], GL_FLOAT, GL_FALSE, m_stride * sizeof(float), (void*)((int)sum* sizeof(float)));
		glEnableVertexAttribArray(i);
		// std::cout <<  "id: " << i  << " size: " << m_layout[i] << " stride: " << total * sizeof(float) << " ptr: " << (sum * sizeof(float));
		// std::cout << "\n";
		sum += m_layout[i];
	}

	//

	// glVertexAttribPointer(id, size, GL_FLOAT, GL_FALSE, t * sizeof(float), (void*)(toosal laendha * sizeof(float)));
	// glEnableVertexAttribArray(id);

}
