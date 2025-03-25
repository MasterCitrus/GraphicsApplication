#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

struct GLFWwindow;

class Camera
{
public:
	Camera() = default;
	Camera(float fov, float aspectRatio, float nearClip, float farClip);

	const glm::mat4& GetViewMatrix() const { return view; }
	glm::mat4 GetProjectionMatrix() const { return projection; }

	glm::vec3& GetPosition() { return position; }
	glm::quat GetOrientation() const;

	glm::vec3 GetUpVector() const;
	glm::vec3 GetRightVector() const;
	glm::vec3 GetForwardVector() const;

	void SetViewportSize(float width, float height);
	void Update(float delta, GLFWwindow* window);

private:
	void UpdateProjection();
	void UpdateView();

private:
	glm::mat4 projection = glm::mat4(1);
	glm::mat4 view;

	glm::vec3 position = { 0.0f, 2.0f, 10.0f };
	float pitch = 0.0f, yaw = 0.0f;

	float fov = 45.0f;
	float aspectRatio = 1.778f;
	float nearClip = 0.1f;
	float farClip = 1000.0f;

	float turnSpeed = 0.25f;
	float cameraSpeed = 10.0f;

	unsigned int width = 1280, height = 720;

};