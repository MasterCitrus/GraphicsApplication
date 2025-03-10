#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/quaternion_float.hpp>

class Transform
{
private:
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::quat rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

	glm::mat4 GetLocalMatrix();

public:

};