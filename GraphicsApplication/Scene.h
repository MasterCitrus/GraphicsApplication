#pragma once
#include "Light.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <list>
#include <vector>


class Camera;
class Instance;
class Object;

const int MAX_LIGHTS = 4;

class Scene
{
protected:

	std::vector<Light*> pointLights;
	std::vector<Object*> objects;
	std::list<Instance*> instances;
	Light sunLight;
	Camera* camera;
	glm::vec2 windowSize;
	glm::vec3 ambientLight;
	glm::vec3 pointLightPositions[MAX_LIGHTS];
	glm::vec3 pointLightColours[MAX_LIGHTS];

public:
	Scene(Camera* camera, glm::vec2 windowSize, Light* light, glm::vec3 ambientLight);
	~Scene();

	void Draw();
	void Update(float delta);
	void AddInstance(Instance* instance);
	void AddObject(Object* object);
	void AddLight(Light* light);

	Camera* GetCamera() { return camera; }
	Light& GetLight() { return sunLight; }

	int GetNumLights() { return (int)pointLights.size(); }

	glm::vec2 GetWindowSize() const { return windowSize; }
	glm::vec3 GetAmbientLight() { return ambientLight; }

	glm::vec3* GetLightPositions() { return pointLightPositions; }
	glm::vec3* GetLightColours() { return pointLightColours; }

	std::vector<Light> GetNearestLights(Object* object);

	std::vector<Light*>& GetPointLights() { return pointLights; }
	std::list<Instance*> GetInstances() { return instances; }
	std::vector<Object*>& GetObjects() { return objects; }
};