#pragma once
#include "Texture.h"
#include <glm/vec3.hpp>

struct Material
{
	Material();
	~Material();

	glm::vec3 Kd;
	glm::vec3 Ka;
	glm::vec3 Ks;
	float shininess;
};