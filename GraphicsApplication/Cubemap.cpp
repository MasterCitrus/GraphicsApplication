#include "Cubemap.h"

#include <glad/glad.h>
#include <filesystem>
#include <iostream>
#include <cassert>

#include "stb_image.h"

Cubemap::Cubemap(const std::string& path)
{
	for( const auto& file : std::filesystem::directory_iterator(path) )
	{
		if( !std::filesystem::is_directory(file) )
			filenames.push_back(file.path().string());
	}

	assert(filenames.size() == 6 && "6 images are required for a cubemap");

	if( !Load() ) std::cout << "Failed to load images\n";
}

Cubemap::Cubemap(Cubemap&& cubemap) noexcept
{
	
	cubemapID = cubemap.cubemapID;
	filenames = cubemap.filenames;

	cubemap.cubemapID = 0;
	filenames = std::vector<std::string>();
}

Cubemap::~Cubemap()
{
	if( cubemapID != 0 )
	{
		glDeleteTextures(1, &cubemapID);
	}
}

Cubemap& Cubemap::operator=(Cubemap&& cubemap) noexcept
{
	if( cubemapID != cubemap.cubemapID )
	{
		cubemapID = cubemap.cubemapID;
		filenames = cubemap.filenames;

		cubemap.cubemapID = 0;
		filenames = std::vector<std::string>();
	}

	return *this;
}

bool Cubemap::Load()
{
	glGenTextures(1, &cubemapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	int width, height, channels;
	for( int i = 0; i < 6; i++ )
	{
		unsigned char* data = stbi_load(filenames[i].c_str(), &width, &height, &channels, 0);
		if( data )
		{
			switch (channels)
			{
			case STBI_grey:
				stbi_set_flip_vertically_on_load(false);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
				break;
			case STBI_grey_alpha:
				stbi_set_flip_vertically_on_load(false);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
				break;
			case STBI_rgb:
				stbi_set_flip_vertically_on_load(false);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
				break;
			case STBI_rgb_alpha:
				stbi_set_flip_vertically_on_load(false);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
				break;
			}			
		}
		else
		{
			std::cout << "Image failed to load: " << filenames[i] << '\n';
			stbi_image_free(data);
			return false;
		}
	}

	return true;
}

void Cubemap::Bind(int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
}
