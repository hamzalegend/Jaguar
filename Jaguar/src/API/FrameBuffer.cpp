#include "FrameBuffer.h"

#include <glad/glad.h>
#include <iostream>

namespace Utils
{
	static bool isDepthFormat(FrameBufferTextureFormat format)
	{
		switch (format)
		{
		case FrameBufferTextureFormat::DEPTH24STENCIL8: return true;
		}
		return false;
	}

	static GLenum TextureTarget(bool multismapled)
	{
		return multismapled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void createTextures(bool multisampled, unsigned int* outID, unsigned int count)
	{
		glCreateTextures(TextureTarget(multisampled), count, outID);

	}

	static void BindTexture(bool multisampled, unsigned int ID)
	{
		glBindTexture(TextureTarget(multisampled), ID);
	}

	static void AttachDepthTexture(unsigned int ID, int samples, GLenum format, GLenum AttachmentType, unsigned int width, unsigned int height)
	{
		bool multisampled = samples > 1;

		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, AttachmentType, TextureTarget(multisampled), ID, 0);
	}

	static void AttachColorTexture(unsigned int ID, int samples, GLenum internalFormat, GLenum format, unsigned int width, unsigned int height, int index)
	{
		bool multisampled = samples > 1;

		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), ID, 0);
	}

}
FrameBuffer::FrameBuffer(const FrameBufferSpecification& spec)
	:m_Spec(spec)
{

	for (auto format : m_Spec.Atachments.Attachments)
	{
		if (!Utils::isDepthFormat(format.TextureFormat))
			m_colorAttachmentSpecs.emplace_back(format);
		else
			m_depthAttachmentSpec = format;
	}

	Invalidate();
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
	glDeleteTextures(1, &m_DepthAttachment);
}

void FrameBuffer::Invalidate()
{
	if (m_ID)
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);

		m_colorAttachments.clear();
		m_DepthAttachment = 0;
	}

	glCreateFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	// Attachments

	bool Multispamle = m_Spec.Samples > 1;

	if (m_colorAttachmentSpecs.size())
	{
		m_colorAttachments.resize(m_colorAttachmentSpecs.size());
		Utils::createTextures(Multispamle, m_colorAttachments.data(), m_colorAttachments.size());

		for (int i = 0; i < m_colorAttachmentSpecs.size(); i++)
		{
			Utils::BindTexture(Multispamle, m_colorAttachments[i]);
			switch (m_colorAttachmentSpecs[i].TextureFormat)
			{
			case FrameBufferTextureFormat::RGBA8: Utils::AttachColorTexture(m_colorAttachments[i], m_Spec.Samples, GL_RGBA8, GL_RGBA, m_Spec.width, m_Spec.height, i);
				break;
			case FrameBufferTextureFormat::RED_INTEGER: Utils::AttachColorTexture(m_colorAttachments[i], m_Spec.Samples, GL_R32I, GL_RED_INTEGER, m_Spec.width, m_Spec.height, i);
				break;
			}
		}
	}

	if (m_depthAttachmentSpec.TextureFormat != FrameBufferTextureFormat::None)
	{
		Utils::createTextures(Multispamle, &m_DepthAttachment, 1);
		Utils::BindTexture(Multispamle, m_DepthAttachment);
		switch (m_depthAttachmentSpec.TextureFormat)
		{
		case FrameBufferTextureFormat::DEPTH24STENCIL8:
			Utils::AttachDepthTexture(m_DepthAttachment, m_Spec.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Spec.width, m_Spec.height);
		}
	}

	if (m_colorAttachments.size() > 1)
	{
		// only sopports up to 4 color attachments (https://www.youtube.com/watch?v=f-GbHye1VFQ&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=106&ab_channel=TheCherno)@ 50:04

		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

		glDrawBuffers(m_colorAttachments.size(), buffers);
	}
	else if (m_colorAttachments.empty())
	{
		glDrawBuffer(GL_NONE);
	}

	if (!glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) std::cout << "[ERROR]:: FrameBuffer Incomplete\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Resize(unsigned int width, unsigned int height)
{
	m_Spec.width = width;
	m_Spec.height = height;

	Invalidate();
}

int FrameBuffer::ReadPixles(unsigned int AtachmentIndex, int x, int y)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + AtachmentIndex);
	int pixelData;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
	return pixelData;
}

static GLenum texFormat2GLtexFormat(FrameBufferTextureFormat format)
{
	switch (format)
	{
	case FrameBufferTextureFormat::RGBA8: return GL_RGBA8;
	case FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
	}

	return 0;
}

void FrameBuffer::ClearAttachment(int index, int value)
{
	auto& spec = m_colorAttachmentSpecs[index];

	glClearTexImage(m_colorAttachments[index], 0, texFormat2GLtexFormat(spec.TextureFormat), GL_INT, &value); // TODO:: abstract GL_INT;
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glViewport(0, 0, m_Spec.width, m_Spec.height);
}

void FrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
