#pragma once
#include <Core/Application.h>

enum class FrameBufferTextureFormat
{
	None = 0,

	// color
	RGBA8,
	RED_INTEGER,

	// depth/stencil
	DEPTH24STENCIL8,

	// default
	Depth = DEPTH24STENCIL8,

};
struct FrameBufferTextureSpecification
{
	FrameBufferTextureSpecification() = default;
	FrameBufferTextureSpecification(FrameBufferTextureFormat format)
		:TextureFormat(format) {}

	FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
};


struct FrameBufferAttachmentSpecification
{
	FrameBufferAttachmentSpecification() = default;
	FrameBufferAttachmentSpecification(const std::initializer_list<FrameBufferTextureSpecification> attachments)
		:Attachments(attachments) {};

	std::vector<FrameBufferTextureSpecification> Attachments;
};

struct FrameBufferSpecification
{
	unsigned int width, height;

	FrameBufferAttachmentSpecification Atachments;

	unsigned int Samples = 1;

	bool SwapchainTarget = false;
};

class JAGUAR_API FrameBuffer
{
public:
	FrameBuffer(const FrameBufferSpecification& spec);
	~FrameBuffer();

	void Invalidate();

	void Resize(unsigned int width, unsigned int height);
	int ReadPixles(unsigned int AtachmentIndex, int x, int y);

	void ClearAttachment(int index, int value);

	void Bind();
	void UnBind();

	inline unsigned int getColorAttachmentID(unsigned int index = 0) { return m_colorAttachments[index]; };
	inline unsigned int getDepthAttachmentID() { return m_DepthAttachment; };

private:
	FrameBufferSpecification m_Spec;

	unsigned int m_ID = 0;
	// unsigned int m_ColorAttachment, m_DepthAttachment_old;

	std::vector<FrameBufferTextureSpecification>m_colorAttachmentSpecs;
	FrameBufferTextureSpecification m_depthAttachmentSpec = FrameBufferTextureFormat::None;

	std::vector<unsigned int> m_colorAttachments;
	unsigned int m_DepthAttachment = 0;


};
