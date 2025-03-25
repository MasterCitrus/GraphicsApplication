#pragma once
#include "Application.h"
#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include "Instance.h"
#include "Skybox.h"
#include "Framebuffer.h"
#include <glm/mat4x4.hpp>
#include <string>

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

	void LoadModel();

protected:
	ShaderProgram shader;
	ShaderProgram normalShader;
	ShaderProgram skyboxShader;

	Scene* scene;

	Framebuffer* framebuffer;

	Skybox* skybox;
	std::string skyboxName;

	glm::vec3 ambientLight;
};