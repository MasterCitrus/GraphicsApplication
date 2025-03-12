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

	shader.loadShader(aie::eShaderStage::VERTEX, "../bin/Shaders/normal.vert");
	shader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/Shaders/normal.frag");

	if (shader.link() == false)
	{
		std::cout << "Shader Error: " << shader.getLastError() << '\n';
		return false;
	}

	model.LoadModel("../Working/soulspear.obj");
	model.LoadMaterials("../Working/soulspear.mtl");

	//model.LoadModel("../Working/Swoop Model.fbx");

	//mesh.InitialiseFromFile("../Working/soulspear.obj");
	//mesh.LoadMaterial("../Working/soulspear.mtl");
	glm::mat4 meshTransform = {
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	Light light;
	light.colour = { 1, 1, 1 };
	light.direction = { 1, 1, -1 };
	ambientLight = { 0.25f, 0.25f, 0.25f };

	scene = new Scene(&camera, glm::vec2(GetWindowWidth(), GetWindowHeight()), &light, ambientLight);
	scene->AddInstance(new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), &model, &shader));
	
	scene->AddLight(Light(glm::vec3(5, 3, 0), glm::vec3(1, 1, 1), 100));
	scene->AddLight(Light(glm::vec3(-5, 3, 0), glm::vec3(0, 1, 0), 100));
	scene->AddLight(Light(glm::vec3(0, 5, 0), glm::vec3(0, 0, 1), 100));


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
	ClearScreen();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	//ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	//if( fullscreen )
	//{
	//	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	//	ImGui::SetNextWindowPos(viewport->WorkPos);
	//	ImGui::SetNextWindowSize(viewport->WorkSize);
	//	ImGui::SetNextWindowViewport(viewport->ID);

	//	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	//	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	//	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	//	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	//}
	//else
	//{
	//	dockspace_flags &= ImGuiDockNodeFlags_PassthruCentralNode;
	//}

	//if( dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode ) window_flags |= ImGuiWindowFlags_NoBackground;

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

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

	//ImGui::PopStyleVar();
	//if(fullscreen) ImGui::PopStyleVar(2);

	//ImGuiIO& io = ImGui::GetIO();

	//if( io.ConfigFlags & ImGuiConfigFlags_DockingEnable )
	//{
	//	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	//	ImGui::DockSpaceOverViewport(dockspace_id, ImGui::GetMainViewport(), dockspace_flags);
	//}
	
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
					model.ResetModel();
					model.LoadModel(path.c_str());
					nfdresult_t result = NFD::OpenDialog(outPath, filterMaterial, 1);
					if( result == NFD_OKAY )
					{
						path = outPath.get();
						unsigned int index = path.find_first_of('\\');
						std::string temp;
						do
						{
							temp = path.substr(index + 1, path.end() - path.begin());
							path.replace(path.begin() + index, path.end(), "/");
							path += temp;
							index = path.find_first_of('\\');
						} while( index != -1 );
						std::cout << path << '\n';
						model.LoadMaterials(path.c_str());
					}
					else if( result == NFD_CANCEL ) std::cout << "Canceled\n";
					else std::cout << "ERROR\n";
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
	ImGui::Text("Mesh Count: %i", model.GetMeshes().size());
	ImGui::End();
}
