#include "Instance.h"
#include "Shader.h"
#include "Model.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include <glm/ext.hpp>

Instance::Instance(glm::mat4 transform, Model* mesh, aie::ShaderProgram* shader) : transform(transform), mesh(mesh), shader(shader)
{
}

Instance::Instance(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale, Model* mesh, aie::ShaderProgram* shader) : mesh(mesh), shader(shader)
{
	transform = MakeTransform(position, eulerAngles, scale);
}

void Instance::Update(float delta)
{
	transform = glm::rotate(transform, glm::radians(45.0f * delta), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Instance::Draw(Camera* camera, float windowWidth, float windowHeight, glm::vec3& ambientLight, Light* light)
{
	shader->bind();

	auto pvm = camera->GetProjectionMatrix(windowWidth, windowHeight) * camera->GetViewMatrix() * transform;

	shader->bindUniform("ProjectionViewModel", pvm);

	shader->bindUniform("ModelMatrix", transform);
	shader->bindUniform("AmbientColour", ambientLight);
	shader->bindUniform("LightColour", light->colour);
	shader->bindUniform("LightDirection", light->direction);

	shader->bindUniform("cameraPosition", camera->GetPosition());

	mesh->Draw(shader);
}

void Instance::Draw(Scene* scene)
{
	shader->bind();

	auto pvm = scene->GetCamera()->GetProjectionMatrix(scene->GetWindowSize().x, scene->GetWindowSize().y) * scene->GetCamera()->GetViewMatrix() * transform;

	shader->bindUniform("ProjectionViewModel", pvm);

	shader->bindUniform("ModelMatrix", transform);
	shader->bindUniform("AmbientColour", scene->GetAmbientLight());
	shader->bindUniform("LightColour", scene->GetLight().colour * scene->GetLight().intensity);
	shader->bindUniform("LightDirection", scene->GetLight().direction);

	shader->bindUniform("CameraPosition", scene->GetCamera()->GetPosition());

	int numLights = scene->GetNumLights();
	shader->bindUniform("numLights", numLights);
	shader->bindUniform("PointLightPosition", numLights, scene->GetLightPositions());
	shader->bindUniform("PointLightColour", numLights, scene->GetLightColours());

	mesh->Draw(shader);
}

glm::mat4 Instance::MakeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
	return glm::translate(glm::mat4(1), position)
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.z), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), scale);
}

void Instance::SetPosition(glm::vec3 position)
{
	transform = glm::translate(transform, position);
}

void Instance::SetRotation(glm::vec3 rotation)
{
	glm::vec4 temp = { rotation, 0.0f };
	transform += transform * temp;
}

void Instance::SetScale(glm::vec3 scale)
{
	transform = glm::scale(transform, scale);
}
