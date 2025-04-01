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
#include <thread>

using aie::Gizmos;

Viewer3D::Viewer3D()
{
}

Viewer3D::~Viewer3D()
{
}

bool Viewer3D::Startup()
{
	camera = new Camera(45.0, (float)GetWindowWidth() / (float)GetWindowHeight(), 0.1f, 1000.0f);

	SetBackgroundColour(.25f, .25f, .25f);

	Gizmos::create(10000, 10000, 0, 0);

	framebuffer = new Framebuffer(GetWindowWidth(), GetWindowHeight());

	instance = this;

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

	//skybox = new Skybox("./Working/Skyboxes/SmallNebulaSpace", &skyboxShader, camera);
	//skyboxName = "SmallNebulaSpace";

	Light light;
	light.colour = { 1, 1, 1 };
	light.direction = { 1, 1, -1 };
	ambientLight = { 0.25f, 0.25f, 0.25f };

	scene = new Scene(camera, glm::vec2(GetWindowWidth(), GetWindowHeight()), &light, ambientLight);
	
	scene->AddLight(new Light(glm::vec3(5, 3, 0), glm::vec3(1, 0, 0), 100));
	scene->AddLight(new Light(glm::vec3(-5, 3, 0), glm::vec3(0, 1, 0), 100));
	scene->AddLight(new Light(glm::vec3(0, 5, 0), glm::vec3(0, 0, 1), 100));


	return true;
}

void Viewer3D::Shutdown()
{
	Gizmos::destroy();
	delete scene;
	if(skybox ) delete skybox;
}

void Viewer3D::Update(float delta)
{
	scene->Update(delta);
}

void Viewer3D::Draw()
{
	ClearScreen(glm::vec3(0.2f, 0.2f, 0.2f));

	framebuffer->Bind();
	glEnable(GL_DEPTH_TEST);
	ClearScreen(glm::vec3(0.2f, 0.2f, 0.2f));
	Gizmos::clear();

	Gizmos::addTransform(glm::mat4(1), 1.0f);

	glm::vec4 white(1);
	glm::vec4 black(0.3, 0.3, 0.3, 1);

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

	glm::mat4 pv = camera->GetProjectionMatrix() * camera->GetViewMatrix();

	Gizmos::draw(pv);

	skybox->Draw();

	framebuffer->Unbind();
}

void Viewer3D::ImGuiDraw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	static bool vsync = true;

	if( ImGui::BeginMainMenuBar() )
	{
		if( ImGui::BeginMenu("File") )
		{
			if( ImGui::MenuItem("Load Model", nullptr) )
			{
				LoadModel();
			}
			if( ImGui::MenuItem("Exit") )
			{
				Quit();
			}
			ImGui::EndMenu();
		}
		if( ImGui::BeginMenu("Options") )
		{
			if( ImGui::Checkbox("Toggle VSync", &vsync) )
			{
				window->SetVSync(vsync);
			}
			ImGui::Checkbox("Toggle ImGui Demo Window", &showDemoWindow);
			ImGui::Checkbox("Toggle App Stats", &showAppStats);
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
	for( int i = 0; i < scene->GetPointLights().size(); i++ )
	{
		char label[128];
		sprintf(label, "Light %d", i);
		if( ImGui::Selectable(label, selectedLight == i, 0) )
		{
			selectedLight = i;
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("LightDetails", ImVec2(0, 200), true);
	if( !scene->GetPointLights().empty() )
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
		ImGui::Text("Scale   ");
		ImGui::SameLine();
		ImGui::DragFloat("##Scale", &scene->GetObjects()[selectedObject]->GetTransform().GetScaleValue(), 0.1f);
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

	ImGui::Begin("World Settings");
	ImGui::Text("Current Skybox: %s", skyboxName.c_str());
	if( ImGui::Button("Change Skybox") )
	{
		LoadSkybox();
	}
	ImGui::SameLine();
	if( ImGui::Button("Remove Skybox") )
	{
		if( skybox )
		{
			delete skybox;
			skybox = nullptr;
		}
	}
	ImGui::End();

	ImGui::Begin("Viewport");
	ImVec2 viewport = ImGui::GetCursorScreenPos();
	ImVec2 availableViewport = ImGui::GetContentRegionAvail();
	ImVec2 windowSize = ImGui::GetWindowSize();

	if( FramebufferSpec spec = framebuffer->GetSpec(); availableViewport.x > 0.0f && availableViewport.y > 0.0f && ( spec.width != availableViewport.x || spec.height != availableViewport.y ) )
	{
		framebuffer->Resize(availableViewport.x, availableViewport.y);
		camera->SetViewportSize(availableViewport.x, availableViewport.y);
	}

	ImGui::Image(framebuffer->GetColourAttachment(), ImVec2(availableViewport.x, availableViewport.y), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();

	if( showAppStats )
	{
		ImGui::SetNextWindowBgAlpha(0.35f);
		ImGui::Begin("App Stats", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDocking);
		ImGui::Text("FPS: %i", fps);
		ImGui::Text("Viewport Size: %i, %i", (unsigned int)availableViewport.x, (unsigned int)availableViewport.y);
		ImGui::Text("Framebuffer Size: %i, %i", framebuffer->GetSpec().width, framebuffer->GetSpec().height);
		ImGui::Text("Window Size: %i, %i", GetWindowWidth(), GetWindowHeight());
		ImGui::Text("Aspect Ratio: %f", camera->GetAspectRatio());
		ImGui::End();
	}
}

void Viewer3D::OnEvent(Event& e)
{
	Application::OnEvent(e);

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN( Viewer3D::OnKeyPressed ));
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN( Viewer3D::OnMouseButtonPressed ));
}

void Viewer3D::LoadModel()
{
	NFD::Guard nfdGuard;
	NFD::UniquePath outPath;
	nfdfilteritem_t filterModel[2] = { { "Wavefront", "obj" }, {"FBX", "fbx"} };
	nfdfilteritem_t filterMaterial[1] = { "Material", "mtl" };

	nfdresult_t result = NFD::OpenDialog(outPath, filterModel, 2);
	if( result == NFD_OKAY )
	{
		std::string path = outPath.get();
		std::cout << path << '\n';

		Model* model = new Model(path.c_str());

		if( path.find(".obj") != -1 )
		{
			nfdresult_t result = NFD::OpenDialog(outPath, filterMaterial, 1);
			if( result == NFD_OKAY )
			{
				path = outPath.get();
				model->LoadMaterials(path.c_str());
			}
			else if( result == NFD_CANCEL ) std::cout << "Model Material Load Canceled\n";
			else std::cout << "Error: " << NFD::GetError() << '\n';
		}
		scene->AddObject(new Object(model, &shader));
	}
	else if( result == NFD_CANCEL ) std::cout << "Model Load Canceled\n";
	else std::cout << "Error: " << NFD::GetError() << '\n';
}

void Viewer3D::LoadSkybox()
{
	NFD::Guard nfdGuard;
	NFD::UniquePath outPath;

	nfdresult_t result = NFD::PickFolder(outPath);
	if( result == NFD_OKAY )
	{
		std::string path = outPath.get();
		if( skybox )
		{
			skybox->SetCubemap(path);
			int index = path.find_last_of("/\\");
			skyboxName = path.substr(index + 1);
		}
		else
		{
			skybox = new Skybox(path, skyboxShader, camera);
			int index = path.find_last_of("/\\");
			skyboxName = path.substr(index + 1);
		}
		
	}
	else if( result == NFD_CANCEL ) {}
	else std::cout << "Error: " << NFD::GetError() << '\n';
}

bool Viewer3D::OnKeyPressed(KeyPressedEvent& e)
{
	switch( e.GetKeyCode() )
	{
	case GLFW_KEY_R:
		camera->SetFocus({ 0.0f, 0.0f, 0.0f });
		break;
	}
	return false;
}

bool Viewer3D::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	return false;
}
