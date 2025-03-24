#pragma once

#include <string>
#include <vector>

class Cubemap
{
public:
	Cubemap(const std::string& path);
	~Cubemap();

	bool Load();

	void Bind(int slot) const;

private:
	std::vector<std::string> filenames;
	unsigned int cubemapID;
};