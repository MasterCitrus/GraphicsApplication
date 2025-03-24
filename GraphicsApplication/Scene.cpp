#include "Scene.h"
#include "Instance.h"
#include "Light.h"
#include "Object.h"
#include <algorithm>

Scene::Scene(Camera* camera, glm::vec2 windowSize, Light* light, glm::vec3 ambientLight) : camera(camera), windowSize(windowSize), sunLight(*light), ambientLight(ambientLight)
{

}

Scene::~Scene()
{
	for( int i = 0; i < objects.size(); i++ ) delete objects[i];
	objects.clear();

	for( int i = 0; i < pointLights.size(); i++ ) delete pointLights[i];
	pointLights.clear();
}

void Scene::Draw()
{
	for( int i = 0; i < objects.size(); i++ )
	{
		std::vector<Light> lights = GetNearestLights(objects[i]);
		for( int i = 0; i < lights.size(); i++ )
		{
			pointLightPositions[i] = lights[i].direction;
			pointLightColours[i] = lights[i].colour * lights[i].intensity;
		}
		objects[i]->Draw(this);
	}

	for( int i = 0; i < pointLights.size(); i++)
	{
		pointLights[i]->Draw();
	}
}

void Scene::Update(float delta)
{
	for( auto it = objects.begin(); it != objects.end(); it++ )
	{
		Object* object = *it;
		object->Update(delta);
	}
}

void Scene::AddInstance(Instance* instance)
{
	instances.push_back(instance);
}

void Scene::AddObject(Object* object)
{
	objects.push_back(object);
}

void Scene::AddLight(Light* light)
{
	pointLights.push_back(light);
}

std::vector<Light> Scene::GetNearestLights(Object* object)
{
	glm::vec3 objectPos = object->GetTransform().GetPosition();

	std::vector<Light> lights;

	for( auto light : pointLights )
	{
		lights.push_back(*light);
	}

	std::sort(lights.begin(), lights.end(), [&objectPos](Light l1, Light l2)
		{
			return glm::distance(objectPos, l1.direction) < glm::distance(objectPos, l2.direction);
		});

	return lights;
}
