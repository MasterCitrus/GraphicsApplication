#pragma once

#include <string>

class Texture
{
public:
	enum Format : unsigned int
	{
		RED = 1,
		RG,
		RGB,
		RGBA
	};

	Texture();
	Texture(const std::string& path, const std::string& type);
	Texture(unsigned int width, unsigned int height, Format format, unsigned char* pixels = nullptr);
	~Texture();

	bool Load(const std::string& path);
	void Create(unsigned int width, unsigned int height, Format format, unsigned char* pixels = nullptr);

	unsigned int GetWidth() const { return width; }
	unsigned int GetHeight() const { return height; }
	unsigned int GetTextureID() const { return textureID; }
	std::string GetPath() const { return path; }
	std::string GetType() const { return type; }

	void Bind(unsigned int slot = 0) const;

private:
	std::string path;
	std::string type;
	unsigned int width, height;
	unsigned int textureID;
	unsigned int format;
	unsigned char* pixels;
};