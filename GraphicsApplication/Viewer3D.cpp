#pragma warning(disable : 4996)
#include "Viewer3D.h"
#include "Camera.h"
#include "Gizmos.h"
#include "Scene.h"
#include "Object.h"
#include "Filesystem.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <nfd/nfd.hpp>

using aie::Gizmos;

std::string GetGLType(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT:
		return "Float";
	case GL_FLOAT_VEC2:
		return "Vec2";
	case GL_FLOAT_VEC3:
		return "Vec3";
	case GL_FLOAT_VEC4:
		return "Vec4";
	case GL_DOUBLE:
		return "Double";
	case GL_DOUBLE_VEC2:
		return "DVec2";
	case GL_DOUBLE_VEC3:
		return "DVec3";
	case GL_DOUBLE_VEC4:
		return "DVec4";
	case GL_INT:
		return "Int";
	case GL_INT_VEC2:
		return "IVec2";
	case GL_INT_VEC3:
		return "IVec3";
	case GL_INT_VEC4:
		return "IVec4";
	case GL_UNSIGNED_INT:
		return "Unsigned Int";
	case GL_UNSIGNED_INT_VEC2:
		return "UVec2";
	case GL_UNSIGNED_INT_VEC3:
		return "UVec3";
	case GL_UNSIGNED_INT_VEC4:
		return "UVec4";
	case GL_BOOL:
		return "Boolean";
	case GL_BOOL_VEC2:
		return "BVec2";
	case GL_BOOL_VEC3:
		return "BVec3";
	case GL_BOOL_VEC4:
		return "BVec4";
	case GL_FLOAT_MAT2:
		return "Matrix2";
	case GL_FLOAT_MAT3:
		return "Matrix3";
	case GL_FLOAT_MAT4:
		return "Matrix4";
	case GL_FLOAT_MAT2x3:
		return "Matrix2x3";
	case GL_FLOAT_MAT2x4:
		return "Matrix2x4";
	case GL_FLOAT_MAT3x2:
		return "Matrix3x2";
	case GL_FLOAT_MAT3x4:
		return "Matrix3x4";
	case GL_FLOAT_MAT4x2:
		return "Matrix4x2";
	case GL_FLOAT_MAT4x3:
		return "Matrix4x3";
	case GL_DOUBLE_MAT2:
		return "DMatrix2";
	case GL_DOUBLE_MAT3:
		return "DMatrix3";
	case GL_DOUBLE_MAT4:
		return "DMatrix4";
	case GL_DOUBLE_MAT2x3:
		return "DMatrix2x3";
	case GL_DOUBLE_MAT2x4:
		return "DMatrix2x4";
	case GL_DOUBLE_MAT3x2:
		return "DMatrix3x2";
	case GL_DOUBLE_MAT3x4:
		return "DMatrix3x4";
	case GL_DOUBLE_MAT4x2:
		return "DMatrix4x2";
	case GL_DOUBLE_MAT4x3:
		return "DMatrix4x3";
	case GL_SAMPLER_1D:
		return "Sampler1D";
	case GL_SAMPLER_2D:
		return "Sampler2D";
	case GL_SAMPLER_3D:
		return "Sampler3D";
	case GL_SAMPLER_CUBE:
		return "SamplerCube";
	case GL_SAMPLER_1D_SHADOW:
		return "Sampler1D Shadow";
	case GL_SAMPLER_2D_SHADOW:
		return "Sampler2D Shadow";
	case GL_SAMPLER_1D_ARRAY:
		return "Sampler1DArray";
	case GL_SAMPLER_2D_ARRAY:
		return "Sampler2DArray";
	case GL_SAMPLER_1D_ARRAY_SHADOW:
		return "Sampler1DArrayShadow";
	case GL_SAMPLER_2D_ARRAY_SHADOW:
		return "Sampler2DArrayShadow";
	case GL_SAMPLER_2D_MULTISAMPLE:
		return "Sampler2DMS";
	case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
		return "Sampler2DMSArray";
	case GL_SAMPLER_CUBE_SHADOW:
		return "SamplerCubeShadow";
	case GL_SAMPLER_BUFFER:
		return "SamplerBuffer";
	case GL_SAMPLER_2D_RECT:
		return "Sampler2DRect";
	case GL_SAMPLER_2D_RECT_SHADOW:
		return "sampelr2DRectShadow";
	case GL_INT_SAMPLER_1D:
		return "ISampler1D";
	case GL_INT_SAMPLER_2D:
		return "ISampler2D";
	case GL_INT_SAMPLER_3D:
		return "ISampler3D";
	case GL_INT_SAMPLER_CUBE:
		return "ISamplerCube";
	case GL_INT_SAMPLER_1D_ARRAY:
		return "ISampler1DArray";
	case GL_INT_SAMPLER_2D_ARRAY:
		return "ISampler2DArray";
	case GL_INT_SAMPLER_2D_MULTISAMPLE:
		return "ISampler2DMS";
	case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
		return "ISampler2DMSArray";
	case GL_INT_SAMPLER_BUFFER:
		return "ISamplerBuffer";
	case GL_INT_SAMPLER_2D_RECT:
		return "ISampler2DRect";
	case GL_UNSIGNED_INT_SAMPLER_1D:
		return "USampler1D";
	case GL_UNSIGNED_INT_SAMPLER_2D:
		return "USampler2D";
	case GL_UNSIGNED_INT_SAMPLER_3D:
		return "USampler3D";
	case GL_UNSIGNED_INT_SAMPLER_CUBE:
		return "USamplerCube";
	case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
		return "USampler1DArray";
	case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
		return "USampler2DArray";
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
		return "USampler2DMS";
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
		return "USampler2DMSArray";
	case GL_UNSIGNED_INT_SAMPLER_BUFFER:
		return "USamplerBuffer";
	case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
		return "USampler2DRect";
	case GL_IMAGE_1D:
		return "Image1D";
	case GL_IMAGE_2D:
		return "Image2D";
	case GL_IMAGE_3D:
		return "Image3D";
	case GL_IMAGE_2D_RECT:
		return "Image2DRect";
	case GL_IMAGE_CUBE:
		return "ImageCube";
	case GL_IMAGE_BUFFER:
		return "ImageBuffer";
	case GL_IMAGE_1D_ARRAY:
		return "Image1DArray";
	case GL_IMAGE_2D_ARRAY:
		return "Image2DArray";
	case GL_IMAGE_2D_MULTISAMPLE:
		return "Image1DMS";
	case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
		return "Image2DMSArray";
	case GL_INT_IMAGE_1D:
		return "IImage1D";
	case GL_INT_IMAGE_2D:
		return "IImage2D";
	case GL_INT_IMAGE_3D:
		return "IImage3D";
	case GL_INT_IMAGE_2D_RECT:
		return "IImage2DRect";
	case GL_INT_IMAGE_CUBE:
		return "IImageCube";
	case GL_INT_IMAGE_BUFFER:
		return "IImageBuffer";
	case GL_INT_IMAGE_1D_ARRAY:
		return "IImage1DArray";
	case GL_INT_IMAGE_2D_ARRAY:
		return "IImage2DArray";
	case GL_INT_IMAGE_2D_MULTISAMPLE:
		return "IImage1DMS";
	case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
		return "IImage2DMSArray";
	case GL_UNSIGNED_INT_IMAGE_1D:
		return "UImage1D";
	case GL_UNSIGNED_INT_IMAGE_2D:
		return "UImage2D";
	case GL_UNSIGNED_INT_IMAGE_3D:
		return "UImage3D";
	case GL_UNSIGNED_INT_IMAGE_2D_RECT:
		return "UImage2DRect";
	case GL_UNSIGNED_INT_IMAGE_CUBE:
		return "UImageCube";
	case GL_UNSIGNED_INT_IMAGE_BUFFER:
		return "UImageBuffer";
	case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
		return "UImage1DArray";
	case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
		return "UImage2DArray";
	case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
		return "UImage1DMS";
	case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
		return "UImage2DMSArray";
	case GL_UNSIGNED_INT_ATOMIC_COUNTER:
		return "AtomicUInt";
	}
}

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

	simpleShader.loadShader(aie::eShaderStage::VERTEX, "./bin/Shaders/simple.vert");
	simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./bin/Shaders/simple.frag");

	skyboxShader.loadShader(aie::eShaderStage::VERTEX, "./bin/Shaders/skybox.vert");
	skyboxShader.loadShader(aie::eShaderStage::FRAGMENT, "./bin/Shaders/skybox.frag");

	if (shader.link() == false)
	{
		std::cout << "Shader Error: " << shader.getLastError() << '\n';
		return false;
	}

	if( simpleShader.link() == false )
	{
		std::cout << "Shader Error: " << simpleShader.getLastError() << '\n';
		return false;
	}

	if( skyboxShader.link() == false )
	{
		std::cout << "Skybox Shader Error: " << skyboxShader.getLastError() << '\n';
		return false;
	}

	Light light;
	light.colour = { 1, 1, 1 };
	light.direction = { 0, -1, -1 };
	light.intensity = 1.0f;
	ambientLight = { 0.25f, 0.25f, 0.25f };

	int count = 0;

	glGetProgramiv(shader.getHandle(), GL_ACTIVE_UNIFORMS, &count);

	const int bufsize = 256;
	char name[bufsize];
	int length;
	int size;
	unsigned int type;
	for (int i = 0; i < count; i++)
	{
		glGetActiveUniform(shader.getHandle(), i, bufsize, &length, &size, &type, name);
		std::cout << "Uniform: " << i << " Type: " << GetGLType(type) << " Name: " << name << '\n';
	}

	scene = new Scene(camera, glm::vec2(GetWindowWidth(), GetWindowHeight()), &light, ambientLight);
	
	scene->AddLight(new Light(glm::vec3(5, 3, 0), glm::vec3(1, 0, 0), 100));
	scene->AddLight(new Light(glm::vec3(-5, 3, 0), glm::vec3(0, 1, 0), 100));
	scene->AddLight(new Light(glm::vec3(0, 5, 0), glm::vec3(0, 0, 1), 100));

	Mesh* mesh = new Mesh();
	mesh->InitialiseQuad();
	mesh->meshMaterial.Kd = { 0.1f, 0.1f, 0.1f };
	Model* quadModel = new Model(mesh);
	Object* quad = new Object(quadModel, &simpleShader);
	quad->GetTransform().SetScale(10);
	quad->GetTransform().SetPosition({ 0.0f, -0.1f, 0.0f });

	scene->AddObject(quad);

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
	if(viewportHovered || ( viewportFocused && viewportHovered ) ) camera->Update(delta);
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

	if(skybox) skybox->Draw();

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

	//ImGui::SeparatorText("Objects");
	//static int selectedObject = 0;
	//ImGui::BeginChild("Model List", ImVec2(70, 200), true);
	//for( int i = 0; i < scene->GetObjects().size(); i++ )
	//{
	//	char label[128];
	//	sprintf(label, "Object %d", i);
	//	if( ImGui::Selectable(label, selectedObject == i, 0) )
	//	{
	//		selectedObject = i;
	//	}
	//}
	//ImGui::EndChild();
	//ImGui::SameLine();
	//ImGui::BeginChild("Object Details", ImVec2(0, 200), true);
	//if( !scene->GetObjects().empty() )
	//{
	//	ImGui::Text("Mesh Count: %i", scene->GetObjects()[selectedObject]->GetModel()->GetMeshes().size());
	//	ImGui::Text("Position");
	//	ImGui::SameLine();
	//	ImGui::DragFloat3("##Position", &scene->GetObjects()[selectedObject]->GetTransform().GetPosition()[0], 0.1f);
	//	ImGui::Text("Rotation");
	//	ImGui::SameLine();
	//	ImGui::DragFloat3("##Rotation", &scene->GetObjects()[selectedObject]->GetTransform().GetRotation()[0], 0.1f);
	//	ImGui::Text("Scale   ");
	//	ImGui::SameLine();
	//	ImGui::DragFloat("##Scale", &scene->GetObjects()[selectedObject]->GetTransform().GetScaleValue(), 0.1f);
	//	if( ImGui::Button("Delete Object") )
	//	{
	//		auto it = scene->GetObjects().begin() + selectedObject;
	//		scene->GetObjects().erase(it);
	//		if( selectedObject > 0 ) selectedObject--;
	//		else selectedObject = 0;
	//	}
	//}
	//ImGui::EndChild();
	ImGui::End();

	ImGui::Begin("World Settings");
	ImGui::Text("Current Skybox: %s", skybox == nullptr ? "" : skyboxName.c_str());
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

	ImGui::Begin("Hierarchy");
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
	if(!scene->GetObjects().empty() )
	{
		int i = 0;
		for( auto object : scene->GetObjects() )
		{

		}
		//ImGui::TreePop();
	}

	ImGui::End();

	ImGui::Begin("Properties");

	ImGui::End();

	ImGui::Begin("Log");

	ImGui::End();

	ImGui::Begin("Viewport");
	ImVec2 viewport = ImGui::GetCursorScreenPos();
	ImVec2 availableViewport = ImGui::GetContentRegionAvail();
	ImVec2 windowSize = ImGui::GetWindowSize();

	viewportHovered = ImGui::IsWindowHovered();
	viewportFocused = ImGui::IsWindowFocused();

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
	if( viewportHovered || ( viewportFocused && viewportHovered ) ) camera->OnEvent(e);

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN( Viewer3D::OnKeyPressed ));
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN( Viewer3D::OnMouseButtonPressed ));
}

void Viewer3D::LoadModel()
{
	std::string path;
	if( Filesystem::LoadFilePath(path, FileType::Model) )
	{
		Model* model = new Model(path.c_str());

		int index = path.find(".obj");

		if( index != -1 )
		{
			path.replace(index, path.size(), ".mtl");
			model->LoadMaterials(path.c_str());
		}
		scene->AddObject(new Object(model, &shader));
	}
	else std::cout << "File load failed or cancelled\n";
}

void Viewer3D::LoadSkybox()
{
	std::string path;
	if( Filesystem::LoadPath(path) )
	{
		int index = path.find_last_of("/\\");
		skyboxName = path.substr(index + 1);
		if( skybox )
		{
			skybox->SetCubemap(path);
		}
		else
		{
			skybox = new Skybox(path, &skyboxShader, camera);
		}
	}
	else std::cout << "Load path failed or cancelled\n";
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
