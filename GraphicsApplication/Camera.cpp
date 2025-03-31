#include "Camera.h"
#include "Application.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>

Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip)
	: fov(fov), aspectRatio(aspectRatio), nearClip(nearClip), farClip(farClip), projection(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
{
	UpdateView();
}

glm::quat Camera::GetOrientation() const
{
	return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
}

glm::vec3 Camera::GetUpVector() const
{
	return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera::GetRightVector() const
{
	return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 Camera::GetForwardVector() const
{
	return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

void Camera::SetViewportSize(float width, float height)
{
	this->width = width;
	this->height = height;

	UpdateProjection();
}

void Camera::Update(float delta, GLFWwindow* window)
{


	glm::vec2 mouseDelta = Application::Get()->GetMouseDelta() * 0.009f;

	if( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) )
	{
		Pan(mouseDelta);
	}
	else if( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) )
	{
		Zoom(mouseDelta.y);
	}
	else if( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) )
	{
		Rotate(mouseDelta);
	}
	

	//if (glfwGetKey(window, GLFW_KEY_R))
	//{
	//	position += GetUpVector() * cameraSpeed * delta;
	//}
	//if (glfwGetKey(window, GLFW_KEY_F))
	//{
	//	position += -GetUpVector() * cameraSpeed * delta;
	//}
	//if (glfwGetKey(window, GLFW_KEY_W))
	//{
	//	position += GetForwardVector() * cameraSpeed * delta;
	//}
	//if (glfwGetKey(window, GLFW_KEY_S))
	//{
	//	position += -GetForwardVector() * cameraSpeed * delta;
	//}
	//if (glfwGetKey(window, GLFW_KEY_A))
	//{
	//	position += -GetRightVector() * cameraSpeed * delta;
	//}
	//if (glfwGetKey(window, GLFW_KEY_D))
	//{
	//	position += GetRightVector() * cameraSpeed * delta;
	//}
	//if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	//{
	//	cameraSpeed = 15.0f;
	//}
	//else
	//{
	//	cameraSpeed = 10.0f;
	//}
	//if (glfwGetMouseButton(window, 1))
	//{
	//	yaw += turnSpeed * mouseDelta.x * delta;
	//	pitch += turnSpeed * mouseDelta.y * delta;
	//}

	UpdateView();
}

void Camera::UpdateProjection()
{
	aspectRatio = (float)width / (float)height;
	projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
}

void Camera::UpdateView()
{
	position = CalculatePosition();

	glm::quat orientation = GetOrientation();
	view = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
	view = glm::inverse(view);
}

void Camera::Pan(const glm::vec2& delta)
{
	auto [x, y] = PanSpeed();
	focalPoint += -GetRightVector() * delta.x * x * distance;
	focalPoint += GetUpVector() * delta.y * y * distance;
}

void Camera::Rotate(const glm::vec2& delta)
{
	float yawSign = GetUpVector().y < 0 ? -1.0f : 1.0f;
	yaw += yawSign * delta.x * turnSpeed;
	pitch += delta.y * turnSpeed;
}

void Camera::Zoom(float delta)
{
	distance -= delta * ZoomSpeed();
	if( distance < 1.0f )
	{
		focalPoint += GetForwardVector();
		distance = 1.0f;
	}
}

std::pair<float, float> Camera::PanSpeed() const
{
	float x = std::min(width / 1000.0f, 2.4f);
	float xFactor = 0.0366f * ( x * x ) - 0.1778f * x + 0.3021f;

	float y = std::min(height / 1000.0f, 2.4f);
	float yFactor = 0.0366f * ( y * y ) - 0.1778f * y + 0.3021f;

	return { xFactor, yFactor };
}

float Camera::ZoomSpeed() const
{
	float distance = this->distance * 0.2f;
	distance = std::max(distance, 0.0f);
	float speed = distance * distance;
	speed = std::min(speed, 100.0f);
	return speed;
}

glm::vec3 Camera::CalculatePosition() const
{
	return focalPoint - GetForwardVector() * distance;
}
