#pragma once


struct FramebufferSpec
{
	unsigned int width = 0, height = 0;
};

class Framebuffer
{
public:
	Framebuffer(unsigned int width, unsigned int height);
	~Framebuffer();

	void Resize(unsigned int width, unsigned int height);

	void Bind() const;
	void Unbind() const;

	unsigned int GetColourAttachment() const { return colourTexture; }
	unsigned int GetDepthAttachment() const { return depthTexture; }
	unsigned int GetFramebufferID() const { return framebufferID; }

	FramebufferSpec& GetSpec() { return spec; }

private:
	unsigned int framebufferID;
	unsigned int renderbufferID;
	unsigned int colourTexture;
	unsigned int depthTexture;
	FramebufferSpec spec;
};