#include "Viewer3D.h"

int main()
{
	auto app = new Viewer3D();

	app->Run("3D Viewer", 1280, 720, false);

	delete app;
}