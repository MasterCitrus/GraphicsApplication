#pragma once
#include "Application.h"
#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include "Instance.h"
#include <glm/mat4x4.hpp>

using aie::ShaderProgram;

class Viewer3D : public Application
{
public:
	Viewer3D();
	virtual ~Viewer3D();

	virtual bool Startup();
	virtual void Shutdown();
	virtual void Update(float delta);
	virtual void Draw();

protected:
	ShaderProgram shader;
	ShaderProgram normalShader;

	Scene* scene;

	Model* model;

	//Mesh mesh;
	glm::vec3 modelPos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 modelRotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 modelScale = { 1.0f, 1.0f, 1.0f };

	glm::vec3 ambientLight;
};