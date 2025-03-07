#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct GLFWwindow;

class Camera
{
private:
	glm::vec3 position;
	float theta;
	float phi;
	float turnSpeed = 0.25f;
	float cameraSpeed = 10.0f;

public:
	Camera();

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);

	glm::vec3& GetPosition() { return position; }

	void Update(float delta, GLFWwindow* window);
};