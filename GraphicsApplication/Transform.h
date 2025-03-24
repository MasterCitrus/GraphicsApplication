#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Transform
{
public:
	Transform() = default;

	void ComputeTransform();

	glm::mat4 GetLocalMatrix() const { return transform; }

	bool GetIsDirty() const { return isDirty; }

	glm::vec3& GetPosition() { return position; }
	glm::vec3& GetRotation() { return rotation; }
	glm::vec3& GetScale() { return scale; }
	float& GetScaleValue() { return scaleValue; }

	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	void SetRotation(float pitch, float yaw, float roll);
	void SetScale(float scale);

private:
	glm::mat4 transform;
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	float scaleValue = 1.0f;

	bool isDirty = false;
};