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

	bool Startup() override;
	void Shutdown() override;
	void Update(float delta) override;
	void Draw() override;
	void ImGuiDraw() override;

	Camera* GetCamera() { return camera; }

	void OnEvent(Event& e) override;

	void LoadModel();
	void LoadSkybox();

protected:
	bool OnKeyPressed(KeyPressedEvent& e) override;
	bool OnMouseButtonPressed(MouseButtonPressedEvent& e) override;

protected:
	ShaderProgram shader;
	ShaderProgram simpleShader;
	ShaderProgram skyboxShader;

	Camera* camera;
	Scene* scene;

	Framebuffer* framebuffer;

	Skybox* skybox;
	std::string skyboxName;

	glm::vec3 ambientLight;

	bool showAppStats = false;
	bool viewportFocused;
	bool viewportHovered;
};