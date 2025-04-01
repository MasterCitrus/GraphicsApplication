#include "Application.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::instance = nullptr;

Application::Application() : window(nullptr), quit(false), fps(0)
{
	
}

Application::~Application()
{
	delete window;
}

void Application::Run(const char* title, unsigned int width, unsigned int height, bool fullscreen)
{
	window = new Window({ title, width, height, fullscreen });
	window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));
	if (window && Startup())
	{
		auto window = (GLFWwindow*)GetWindow().GetNativeWindow();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");

		double prevTime = glfwGetTime();
		double currTime = 0;
		double deltaTime = 0;
		unsigned int frames = 0;
		double fpsInterval = 0;

		while (!quit)
		{
			currTime = glfwGetTime();
			deltaTime = currTime - prevTime;
			if (deltaTime > 0.1f) deltaTime = 0.1f;

			prevTime = currTime;


			if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) continue;

			frames++;
			fpsInterval += deltaTime;
			if (fpsInterval >= 1.f)
			{
				fps = frames;
				frames = 0;
				fpsInterval -= 1.0f;
			}

			Update((float)deltaTime);

			camera->Update((float)deltaTime, window);

			Draw();

			if( showDemoWindow )
			{
				ImGui::ShowDemoWindow();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
			{
				GLFWwindow* backup_current_context = window;
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			this->window->Update();
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	Shutdown();
}

void Application::ClearScreen(glm::vec3 colour)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(colour.x, colour.y, colour.z, 1.0f);
}

void Application::SetBackgroundColour(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

bool Application::HasWindowClosed()
{
	auto window = (GLFWwindow*)GetWindow().GetNativeWindow();
	return glfwWindowShouldClose(window) == GLFW_TRUE;
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

	camera->OnEvent(e);
}

unsigned int Application::GetWindowWidth() const
{
	return window->GetWidth();
}

unsigned int Application::GetWindowHeight() const
{
	return window->GetHeight();
}

float Application::GetTime() const
{
	return (float)glfwGetTime();
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	quit = true;
	return true;
}
