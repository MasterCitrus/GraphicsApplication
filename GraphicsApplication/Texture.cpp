#include "glad/glad.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture() : path("none"), width(0), height(0), format(0), pixels(nullptr)
{
}

Texture::Texture(const std::string& path, const std::string& type) : path("none"), type(type), width(0), height(0), format(0), pixels(nullptr)
{
	Load(path);
}

Texture::Texture(unsigned int width, unsigned int height, Format format, unsigned char* pixels) : path("none"), width(0), height(0), format(0), pixels(nullptr)
{
	Create(width, height, format, pixels);
}

Texture::~Texture()
{
	if( textureID != 0 ) glDeleteTextures(1, &textureID);
	if( pixels ) stbi_image_free(pixels);
}

bool Texture::Load(const std::string& path)
{
	if( textureID != 0 )
	{
		glDeleteTextures(1, &textureID);
		textureID = 0;
		width = 0;
		height = 0;
		format = 0;
		pixels = nullptr;
		this->path = "none";
	}

	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);

	if( pixels )
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);

		switch( channels )
		{
		case STBI_grey:
			format = RED;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
			break;
		case STBI_grey_alpha:
			format = RG;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, pixels);
			break;
		case STBI_rgb:
			format = RGB;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
			break;
		case STBI_rgb_alpha:
			format = RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			break;
		default:
			break;
		}

		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		this->width = (unsigned int)width;
		this->height = (unsigned int)height;
		this->path = path;

		return true;
	}
	return false;
}

void Texture::Create(unsigned int width, unsigned int height, Format format, unsigned char* pixels)
{
	if( textureID != 0 )
	{
		glDeleteTextures(1, &textureID);
		textureID = 0;
		path = "none";
	}

	this->width = width;
	this->height = height;
	this->format = format;

	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	switch( format )
	{
	case RED:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
		break;
	case RG:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, pixels);
		break;
	case RGB:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		break;
	case RGBA:
	default:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		break;
	}
}

void Texture::Bind(unsigned int slot) const
{
	glBindTextureUnit(slot, textureID);
}