#include "Filesystem.h"
#include <iostream>

bool Filesystem::LoadFilePath(std::string& path, FileType type)
{
	NFD::Guard nfdGuard;
	NFD::UniquePath outPath;
	nfdresult_t result;

	switch( type )
	{
	case FileType::Model:
		result = NFD::OpenDialog(outPath, modelFileTypes, 5, defaultLocation.c_str());
		if( result == NFD_OKAY )
		{
			path = outPath.get();
			return true;
		}
		else if( result == NFD_CANCEL )
		{
			std::cout << "Model Load Canceled\n";
			return false;
		}
		else
		{
			std::cout << "Error: " << NFD::GetError() << '\n';
			return false;
		}
		break;
	case FileType::Image:
		result = NFD::OpenDialog(outPath, imageFileTypes, 3, defaultLocation.c_str());
		if( result == NFD_OKAY )
		{
			path = outPath.get();
			return true;
		}
		else if( result == NFD_CANCEL )
		{
			std::cout << "Model Load Canceled\n";
			return false;
		}
		else
		{
			std::cout << "Error: " << NFD::GetError() << '\n';
			return false;
		}
		break;
	default:
		return false;
	}
	return false;
}

bool Filesystem::LoadPath(std::string& path)
{
	NFD::Guard nfdGuard;
	NFD::UniquePath outPath;

	nfdresult_t result = NFD::PickFolder(outPath, defaultLocation.c_str());
	if( result == NFD_OKAY )
	{
		path = outPath.get();
		return true;
	}
	else if( result == NFD_CANCEL )
	{
		std::cout << "Path pick cancelled\n";
		return false;
	}
	else
	{
		std::cout << "Error: " << NFD::GetError() << '\n';
		return false;
	}
}
