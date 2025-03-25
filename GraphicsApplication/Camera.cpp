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

	glm::vec2 mouseDelta = Application::Get()->GetMouseDelta();

	if (glfwGetKey(window, GLFW_KEY_R))
	{
		position += GetUpVector() * cameraSpeed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_F))
	{
		position += -GetUpVector() * cameraSpeed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		position += GetForwardVector() * cameraSpeed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		position += -GetForwardVector() * cameraSpeed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		position += -GetRightVector() * cameraSpeed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		position += GetRightVector() * cameraSpeed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		cameraSpeed = 15.0f;
	}
	else
	{
		cameraSpeed = 10.0f;
	}
	if (glfwGetMouseButton(window, 1))
	{
		yaw += turnSpeed * mouseDelta.x;
		pitch += turnSpeed * mouseDelta.y;
	}
}

void Camera::UpdateProjection()
{
	aspectRatio = width / height;
	projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
}

void Camera::UpdateView()
{
	glm::quat orientation = GetOrientation();
	view = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
	view = glm::inverse(view);
}
