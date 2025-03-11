#include "Scene.h"
#include "Instance.h"
#include "Light.h"

Scene::Scene(Camera* camera, glm::vec2 windowSize, Light* light, glm::vec3 ambientLight) : camera(camera), windowSize(windowSize), sunLight(*light), ambientLight(ambientLight)
{

}

Scene::~Scene()
{
	for (auto it = instances.begin(); it != instances.end(); it++)
	{
		delete* it;
	}
}

void Scene::Draw()
{
	for (int i = 0; i < pointLights.size(); i++)
	{
		pointLightPositions[i] = pointLights[i].direction;
		pointLightColours[i] = pointLights[i].colour * pointLights[i].intensity;
		pointLights[i].Draw();
	}

	for (auto it = instances.begin(); it != instances.end(); it++)
	{
		Instance* instance = *it;
		instance->Draw(this);
	}
}

void Scene::AddInstance(Instance* instance)
{
	instances.push_back(instance);
}

void Scene::AddLight(Light light)
{
	pointLights.push_back(light);
}
