#pragma once

#include <string>
#include <vector>

class Cubemap
{
public:
	Cubemap(const std::string& path);
	Cubemap(Cubemap& cubemap) = delete;
	Cubemap(Cubemap&& cubemap);
	~Cubemap();

	Cubemap& operator=(Cubemap& cubemap) = delete;
	Cubemap& operator=(Cubemap&& cubemap) noexcept;

	bool Load();

	void Bind(int slot) const;

private:
	std::vector<std::string> filenames;
	unsigned int cubemapID;
};