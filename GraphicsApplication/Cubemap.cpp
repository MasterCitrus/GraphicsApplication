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

	assert(filenames.size() == 6, "6 images are required for a cubemap");

	if( !Load() ) std::cout << "Failed to load images\n";
}

Cubemap::~Cubemap()
{

}

bool Cubemap::Load()
{
	glGenTextures(1, &cubemapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

	int width, height, channels;
	for( int i = 0; i < 6; i++ )
	{
		unsigned char* data = stbi_load(filenames[i].c_str(), &width, &height, &channels, 0);
		if( data )
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Image failed to load: " << filenames[i] << '\n';
			stbi_image_free(data);
			return false;
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return true;
}

void Cubemap::Bind(int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
}
