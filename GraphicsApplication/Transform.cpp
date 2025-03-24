#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

void Transform::ComputeTransform()
{
	transform = glm::translate(glm::mat4(1), position)
		* glm::toMat4(glm::quat(rotation))
		* glm::scale(glm::mat4(1), scale * scaleValue);
}

void Transform::SetPosition(glm::vec3 position)
{
	this->position = position;
	isDirty = true;
}

void Transform::SetRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
	isDirty = true;
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
	rotation.x = pitch;
	rotation.y = yaw;
	rotation.z = roll;
	isDirty = true;
}

void Transform::SetScale(float scale)
{
	this->scale *= scale;
	isDirty = true;
}
