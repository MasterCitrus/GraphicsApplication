#pragma warning(disable : 4996)
#include "Viewer3D.h"
#include "Camera.h"
#include "Gizmos.h"
#include "Scene.h"
#include "Object.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <nfd/nfd.hpp>

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

	shader.loadShader(aie::eShaderStage::VERTEX, "./bin/Shaders/phong.vert");
	shader.loadShader(aie::eShaderStage::FRAGMENT, "./bin/Shaders/phong.frag");

	skyboxShader.loadShader(aie::eShaderStage::VERTEX, "./bin/Shaders/skybox.vert");
	skyboxShader.loadShader(aie::eShaderStage::FRAGMENT, "./bin/Shaders/skybox.frag");

	if (shader.link() == false)
	{
		std::cout << "Shader Error: " << shader.getLastError() << '\n';
		return false;
	}

	if( skyboxShader.link() == false )
	{
		std::cout << "Skybox Shader Error: " << skyboxShader.getLastError() << '\n';
		return false;
	}

	//model->LoadModel("./Working/soulspear.obj");
	//model->LoadMaterials("./Working/soulspear.mtl");

	//model->LoadModel("./Working/Person.fbx");

	//mesh.InitialiseFromFile("../Working/soulspear.obj");
	//mesh.LoadMaterial("../Working/soulspear.mtl");
	//glm::mat4 meshTransform = {
	//	1.f, 0.f, 0.f, 0.f,
	//	0.f, 1.f, 0.f, 0.f,
	//	0.f, 0.f, 1.f, 0.f,
	//	0.f, 0.f, 0.f, 1.f
	//};

	skybox = new Skybox("./Working/Skyboxes/Ocean", &skyboxShader, &camera);

	Light light;
	light.colour = { 1, 1, 1 };
	light.direction = { 1, 1, -1 };
	ambientLight = { 0.25f, 0.25f, 0.25f };

	scene = new Scene(&camera, glm::vec2(GetWindowWidth(), GetWindowHeight()), &light, ambientLight);
	//scene->AddInstance(new Instance(modelPos, modelRotation, modelScale, model, &shader));
	
	scene->AddLight(new Light(glm::vec3(5, 3, 0), glm::vec3(1, 1, 1), 100));
	scene->AddLight(new Light(glm::vec3(-5, 3, 0), glm::vec3(0, 1, 0), 100));
	scene->AddLight(new Light(glm::vec3(0, 5, 0), glm::vec3(0, 0, 1), 100));


	return true;
}

void Viewer3D::Shutdown()
{
	Gizmos::destroy();
	delete scene;
}

void Viewer3D::Update(float delta)
{
	scene->Update(delta);
}

void Viewer3D::Draw()
{
	ClearScreen(glm::vec3(0.2f, 0.2f, 0.2f));

	Gizmos::clear();

	Gizmos::addTransform(glm::mat4(1), 1.0f);

	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);

	for( int i = 0; i < 21; ++i )
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, 10),
			glm::vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(glm::vec3(10, 0, -10 + i),
			glm::vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	scene->Draw();

	glm::mat4 pv = camera.GetProjectionMatrix((float)GetWindowWidth(), (float)GetWindowHeight()) * camera.GetViewMatrix();

	Gizmos::draw(pv);

	skybox->Draw();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	if( ImGui::BeginMainMenuBar() )
	{
		if( ImGui::BeginMenu("File") )
		{
			if( ImGui::MenuItem("Import Model", nullptr) )
			{
				NFD::Guard nfdGuard;
				NFD::UniquePath outPath;
				nfdfilteritem_t filterModel[1] = { "Wavefront", "obj" };
				nfdfilteritem_t filterMaterial[1] = { "Material", "mtl" };

				nfdresult_t result = NFD::OpenDialog(outPath, filterModel, 1);
				if( result == NFD_OKAY )
				{
					std::string path = outPath.get();
					std::cout << path << '\n';

					Model* model = new Model(path.c_str());
					
					if( path.find(".obj") )
					{
						nfdresult_t result = NFD::OpenDialog(outPath, filterMaterial, 1);
						if( result == NFD_OKAY )
						{
							path = outPath.get();
							model->LoadMaterials(path.c_str());
						}
					}
					else if( result == NFD_CANCEL ) std::cout << "Canceled\n";
					else std::cout << "ERROR\n";
					scene->AddObject(new Object(model, &shader));
				}
				else if( result == NFD_CANCEL ) std::cout << "Canceled\n";
				else std::cout << "ERROR\n";
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

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
	ImGui::SeparatorText("Add Lights");
	static glm::vec3 lightPosition = { 0.0f, 0.0f, 0.0f };
	ImGui::Text("Light Position ");
	ImGui::SameLine();
	ImGui::InputFloat3("##NewLightPosition", &lightPosition[0]);
	static glm::vec3 lightColour = { 1.0f, 1.0f, 1.0f };
	ImGui::Text("Light Colour   ");
	ImGui::SameLine();
	ImGui::ColorEdit3("##NewLightColour", &lightColour[0]);
	static float lightIntensity = 0.0f;
	ImGui::Text("Light Intensity");
	ImGui::SameLine();
	ImGui::SliderFloat("##NewLightIntensity", &lightIntensity, 0.0f, 100.f);
	if( ImGui::Button("Add Point Light") )
	{
		Light* light = new Light(lightPosition, lightColour, lightIntensity);
		scene->AddLight(light);
	}
		
	ImGui::SeparatorText("Point Lights");
	ImGui::BeginChild("LightList", ImVec2(70, 200), true);
	static int selectedLight = 0;
	for (int i = 0; i < scene->GetPointLights().size(); i++)
	{
		char label[128];
		sprintf(label, "Light %d", i);
		if (ImGui::Selectable(label, selectedLight == i, 0))
		{
			selectedLight = i;
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("LightDetails", ImVec2(0, 200), true);
	if(!scene->GetPointLights().empty() )
	{
		ImGui::Text("Light Position ");
		ImGui::SameLine();
		ImGui::DragFloat3("##LightPosition", &scene->GetPointLights()[selectedLight]->direction[0], 0.1f, -100.0f, 100.0f);
		ImGui::Text("Light Colour   ");
		ImGui::SameLine();
		ImGui::ColorEdit3("##LightColour", &scene->GetPointLights()[selectedLight]->colour[0]);
		ImGui::Text("Light Intensity");
		ImGui::SameLine();
		ImGui::SliderFloat("##LightIntensity", &scene->GetPointLights()[selectedLight]->intensity, 0.0f, 100.f);
		ImGui::Text("Debug Draw");
		ImGui::SameLine();
		ImGui::Checkbox("##DebugDraw", &scene->GetPointLights()[selectedLight]->debug);
		if( ImGui::Button("Delete Light") )
		{
			auto it = scene->GetPointLights().begin() + selectedLight;
			scene->GetPointLights().erase(it);
			if( selectedLight > 0 ) selectedLight--;
			else selectedLight = 0;
		}
	}
	ImGui::EndChild();

	ImGui::SeparatorText("Objects");
	static int selectedObject = 0;
	ImGui::BeginChild("Model List", ImVec2(70, 200), true);
	for( int i = 0; i < scene->GetObjects().size(); i++ )
	{
		char label[128];
		sprintf(label, "Object %d", i);
		if( ImGui::Selectable(label, selectedObject == i, 0) )
		{
			selectedObject = i;
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Object Details", ImVec2(0, 200), true);
	if( !scene->GetObjects().empty() )
	{
		ImGui::Text("Mesh Count: %i", scene->GetObjects()[selectedObject]->GetModel()->GetMeshes().size());
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::DragFloat3("##Position", &scene->GetObjects()[selectedObject]->GetTransform().GetPosition()[0], 0.1f);
		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::DragFloat3("##Rotation", &scene->GetObjects()[selectedObject]->GetTransform().GetRotation()[0], 0.1f);
		ImGui::Text("Scale");
		ImGui::SameLine();
		ImGui::DragFloat("##Scale   ", &scene->GetObjects()[selectedObject]->GetTransform().GetScaleValue(), 0.1f);
		if( ImGui::Button("Delete Object") )
		{
			auto it = scene->GetObjects().begin() + selectedObject;
			scene->GetObjects().erase(it);
			if( selectedObject > 0 ) selectedObject--;
			else selectedObject = 0;
		}
	}
	ImGui::EndChild();
	ImGui::End();
	
}
