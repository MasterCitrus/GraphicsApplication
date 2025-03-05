#pragma once
#include "Application.h"
#include <glm/mat4x4.hpp>

class Viewer3D : public Application
{
public:
	Viewer3D();
	virtual ~Viewer3D();

	virtual bool Startup();
	virtual void Shutdown();
	virtual void Update(float delta);
	virtual void Draw();

protected:
	glm::mat4 projection;
	glm::mat4 view;
};