#include "Camera.h"
#include "Application.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>

Camera::Camera()
{
	theta = -90.0f;
	phi = -10.0f;
	position = { 0, 2, 10 };
}

glm::mat4 Camera::GetViewMatrix()
{
	float thetaR = glm::radians(theta);
	float phiR = glm::radians(phi);
	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	return glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float w, float h)
{
	return glm::perspective(glm::pi<float>() * 0.25f, w / h, 0.1f, 1000.f);
}

void Camera::Update(float delta, GLFWwindow* window)
{
	if (phi > 70.0f) phi = 70.0f;
	else if (phi < -70.0f) phi = -70.0f;

	float thetaR = glm::radians(theta);
	float phiR = glm::radians(phi);

	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
	glm::vec3 up(0, 1, 0);

	glm::vec2 mouseDelta = Application::Get()->GetMouseDelta();

	if (glfwGetKey(window, GLFW_KEY_R))
	{
		position += up * cameraSpeed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_F))
	{
		position += -up * cameraSpeed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		position += forward * cameraSpeed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		position += -forward * cameraSpeed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		position += -right * cameraSpeed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		position += right * cameraSpeed * delta;
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
		theta += turnSpeed * mouseDelta.x;
		phi -= turnSpeed * mouseDelta.y;
	}
}