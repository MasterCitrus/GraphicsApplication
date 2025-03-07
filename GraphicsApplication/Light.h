#pragma once
#include <glm/vec3.hpp>

struct Light
{
	Light() = default;
	Light(glm::vec3 position, glm::vec3 colour, float intesity);

	glm::vec3 direction;
	glm::vec3 colour;

	float intensity = 0.0f;
};