#pragma once
#include <nfd/nfd.hpp>
#include <string>
#include <filesystem>

static nfdfilteritem_t modelFileTypes[] = { {"Wavefront", "obj"}, {"FBX", "fbx"} };
static nfdfilteritem_t imageFileTypes[] = { {"JPEG", "jpg"}, {"PNG", "png"}, {"Targa", "tga"} };

static std::string defaultLocation = std::filesystem::current_path().string();

enum class FileType
{
	Model = 0,
	Image
};

namespace Filesystem
{
	bool LoadFilePath(std::string& path, FileType type);
	bool LoadPath(std::string& path);
}