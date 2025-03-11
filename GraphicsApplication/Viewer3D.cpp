#pragma warning(disable : 4996)
#include "Viewer3D.h"
#include "Camera.h"
#include "Gizmos.h"
#include "Scene.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>

using aie::Gizmos;

Viewer3D::Viewer3D()
{
}

Viewer3D::~Viewer3D()
{
}

bool Viewer3D::Startup()
{
	SetBackgroundColour(.25f, .25f, .25f);

	Gizmos::create(10000, 10000, 0, 0);

	instance = this;
	glfwSetCursorPosCallback(window, &Application::SetMousePosition);

	shader.loadShader(aie::eShaderStage::VERTEX, "../bin/Shaders/normal.vert");
	shader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/Shaders/normal.frag");

	if (shader.link() == false)
	{
		std::cout << "Shader Error: " << shader.getLastError() << '\n';
		return false;
	}

	//model.LoadModel("../Working/soulspear.obj");
	//model.LoadMaterials("../Working/soulspear.mtl");

	model.LoadModel("../Working/Swoop Model.fbx");

	//mesh.InitialiseFromFile("../Working/soulspear.obj");
	//mesh.LoadMaterial("../Working/soulspear.mtl");
	glm::mat4 meshTransform = {
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	glm::scale(meshTransform, { 0.000001f, 0.000001f, 0.000001f });

	Light light;
	light.colour = { 1, 1, 1 };
	light.direction = { 1, 1, -1 };
	ambientLight = { 0.25f, 0.25f, 0.25f };

	scene = new Scene(&camera, glm::vec2(GetWindowWidth(), GetWindowHeight()), &light, ambientLight);
	scene->AddInstance(new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(3, 3, 3), &model, &shader));
	
	scene->AddLight(Light(glm::vec3(5, 3, 0), glm::vec3(1, 0, 0), 100));
	scene->AddLight(Light(glm::vec3(-5, 3, 0), glm::vec3(0, 1, 0), 100));


	return true;
}

void Viewer3D::Shutdown()
{
	Gizmos::destroy();
	delete scene;
}

void Viewer3D::Update(float delta)
{
	float time = GetTime();

}

void Viewer3D::Draw()
{
	ClearScreen();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	glm::mat4 pv = camera.GetProjectionMatrix((float)GetWindowWidth(), (float)GetWindowHeight()) * camera.GetViewMatrix();


	//shader.bind();

	//shader.bindUniform("AmbientColour", ambientLight);
	//shader.bindUniform("LightColour", light.colour);
	//shader.bindUniform("LightDirection", light.direction);

	//shader.bindUniform("cameraPosition", camera.GetPosition());

	//auto pvm = pv * meshTransform;
	//shader.bindUniform("ProjectionViewModel", pvm);

	//shader.bindUniform("ModelMatrix", meshTransform);

	Gizmos::clear();

	Gizmos::addTransform(glm::mat4(1), 1.0f);

	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, 10),
						glm::vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(glm::vec3(10, 0, -10 + i),
						glm::vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	//mesh.ApplyMaterial(&shader);
	//mesh.Draw();
	scene->Draw();
	//spearInstance->Draw(&camera, GetWindowWidth(), GetWindowHeight(), ambientLight, &light);

	Gizmos::draw(pv);

	ImGui::Begin("Light Settings");
	ImGui::SeparatorText("Sunlight");
	ImGui::Text("Sunlight Direction");
	ImGui::SameLine();
	ImGui::DragFloat3("##SunlightDirection", &scene->GetLight().direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::Text("Sunlight Colour   ");
	ImGui::SameLine();
	ImGui::ColorEdit3("##SunlightColour", &scene->GetLight().colour[0]);
	ImGui::Text("Sunlight Intensity");
	ImGui::SameLine();
	ImGui::SliderFloat("##SunlightIntensity", &scene->GetLight().intensity, 0.0f, 100.0f);
	ImGui::SeparatorText("Point Lights");
	ImGui::BeginChild("LightList", ImVec2(70, 200), true);
	static int selected = 0;
	for (int i = 0; i < scene->GetPointLights().size(); i++)
	{
		char label[128];
		sprintf(label, "Light %d", i);
		if (ImGui::Selectable(label, selected == i, 0))
		{
			selected = i;
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("LightDetails", ImVec2(0, 200), true);
	ImGui::Text("Light Position ");
	ImGui::SameLine();
	ImGui::DragFloat3("##LightPosition", &scene->GetPointLights()[selected].direction[0], 0.1f, -100.0f, 100.0f);
	ImGui::Text("Light Colour   ");
	ImGui::SameLine();
	ImGui::ColorEdit3("##LightColour", &scene->GetPointLights()[selected].colour[0]);
	ImGui::Text("Light Intensity");
	ImGui::SameLine();
	ImGui::SliderFloat("##LightIntensity", &scene->GetPointLights()[selected].intensity, 0.0f, 100.f);
	ImGui::Text("Debug Draw");
	ImGui::SameLine();
	ImGui::Checkbox("##DebugDraw", &scene->GetPointLights()[selected].debug);
	ImGui::EndChild();
	ImGui::SeparatorText("Model Details");
	std::string meshCount = std::to_string(model.GetMeshes().size());
	ImGui::Text(meshCount.c_str());
	ImGui::End();
}
