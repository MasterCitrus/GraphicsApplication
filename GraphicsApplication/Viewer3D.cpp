#include "Viewer3D.h"
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using aie::Gizmos;

Viewer3D::Viewer3D()
{
}

Viewer3D::~Viewer3D()
{
}

bool Viewer3D::Startup()
{
	SetBackgroundColour(.25f, .25f, .25f);

	Gizmos::create(10000, 10000, 0, 0);

	view = glm::lookAt(glm::vec3(10), glm::vec3(0), glm::vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, GetWindowWidth() / (float)GetWindowHeight(), 0.1f, 1000.f);

	return true;
}

void Viewer3D::Shutdown()
{
	Gizmos::destroy();
}

void Viewer3D::Update(float delta)
{
	float time = GetTime();

	view = glm::lookAt(glm::vec3(glm::sin(time) * 10, 10, glm::cos(time) * 10), glm::vec3(0), glm::vec3(0, 1, 0));

	Gizmos::clear();

	Gizmos::addTransform(glm::mat4(1));

	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, 10),
						glm::vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(glm::vec3(10, 0, -10 + i),
						glm::vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

}

void Viewer3D::Draw()
{
	ClearScreen();

	projection = glm::perspective(glm::pi<float>() * 0.25f, GetWindowWidth() / (float)GetWindowHeight(), 0.1f, 1000.f);

	Gizmos::draw(projection * view);
}
