#include "glad/glad.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture() : path("none"), textureID(0), width(0), height(0), format(0), data(nullptr)
{
}

Texture::Texture(const std::string& path, const std::string& type) : path(path), type(type), textureID(0), width(0), height(0), format(0), data(nullptr)
{
	Load(path);
}

Texture::Texture(unsigned int width, unsigned int height, Format format, unsigned char* data) : path("none"), textureID(0), width(0), height(0), format(0), data(nullptr)
{
	Create(width, height, format, data);
}

Texture::Texture(const Texture& other) : Texture(other.path, other.type)
{
	
}

Texture::Texture(Texture&& other) noexcept
{
	path = std::exchange(other.path, 0);
	type = std::exchange(other.type, 0);
	textureID = std::exchange(other.textureID, 0);
	width = std::exchange(other.width, 0);
	height = std::exchange(other.height, 0);
	format = std::exchange(other.format, 0);
	data = std::exchange(other.data, nullptr);
}

Texture::~Texture()
{
	if( textureID != 0 ) glDeleteTextures(1, &textureID);
	if( data ) stbi_image_free(data);
}

Texture Texture::operator=(const Texture& other)
{
	return *this = Texture(other);
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	std::swap(path, other.path);
	std::swap(type, other.type);
	std::swap(textureID, other.textureID);
	std::swap(width, other.width);
	std::swap(height, other.height);
	std::swap(format, other.format);
	std::swap(data, other.data);
	return *this;
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
		data = nullptr;
		this->path = "none";
	}

	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	if( data )
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);

		switch( channels )
		{
		case STBI_grey:
			format = RED;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
			break;
		case STBI_grey_alpha:
			format = RG;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
			break;
		case STBI_rgb:
			format = RGB;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case STBI_rgb_alpha:
			format = RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		default:
			break;
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		this->width = (unsigned int)width;
		this->height = (unsigned int)height;
		this->path = path;

		return true;
	}
	return false;
}

void Texture::Create(unsigned int width, unsigned int height, Format format, unsigned char* data)
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		break;
	case RG:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
		break;
	case RGB:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		break;
	case RGBA:
	default:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		break;
	}
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}