#pragma once
#include "Texture.h"
#include <glm/vec3.hpp>

using aie::Texture;

class Material
{
	Texture mapKd;
	Texture mapKs;
	Texture mapBump;

	glm::vec3 Kd;
	glm::vec3 Ka;
	glm::vec3 Ks;
	float shininess;
};