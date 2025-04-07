#pragma once
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

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

	float GetAspectRatio() const { return aspectRatio; }

	void SetViewportSize(float width, float height);
	void SetFocus(const glm::vec3& focalPoint);

	void Update(float delta);
	void OnEvent(Event& e);

	bool OnMouseScroll(MouseScrolledEvent& e);
	bool OnKeyPress(KeyPressedEvent& e);
	bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

private:
	void UpdateProjection();
	void UpdateView();

	void Pan(const glm::vec2& delta);
	void Rotate(const glm::vec2& delta);
	void Zoom(float delta);

	std::pair<float, float> PanSpeed() const;
	float ZoomSpeed() const;

	glm::vec3 CalculatePosition() const;

private:
	glm::mat4 projection = glm::mat4(1);
	glm::mat4 view;

	glm::vec3 focalPoint = { 0.0f, 0.0f, 0.0f };
	glm::vec3 position = { 0.0f, 2.0f, 10.0f };

	glm::vec2 lastMousePos = { 0.0f, 0.0f };

	float pitch = glm::radians( 30.0f ), yaw = 0.0f;
	float distance = 10.0f;

	float fov = 45.0f;
	float aspectRatio = 1.778f;
	float nearClip = 0.1f;
	float farClip = 1000.0f;

	float turnSpeed = 0.25f;
	float cameraSpeed = 10.0f;

	unsigned int width = 1280, height = 720;

};