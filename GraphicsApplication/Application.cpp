#include "Application.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

Application* Application::instance = nullptr;

Application::Application() : window(nullptr), quit(false), fps(0)
{
	
}

Application::~Application()
{
	
}

void Application::Run(const char* title, int width, int height, bool fullscreen)
{

	if (CreateWindow(title, width, height, fullscreen) && Startup())
	{
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
			if (fpsInterval >= 0.1f)
			{
				fps = frames;
				frames = 0;
				fpsInterval -= 1.0f;
			}

			Update((float)deltaTime);

			camera->Update((float)deltaTime, window);

			Draw();

			lastMousePos = mousePos;

			if (glfwGetKey(window, GLFW_KEY_ESCAPE)) Quit();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			glfwSwapBuffers(window);
			glfwPollEvents();

			quit = quit || glfwWindowShouldClose(window) == GLFW_TRUE;
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	Shutdown();
	DestroyWindow();
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

void Application::SetShowCursor(bool visible)
{
	//ShowCursor(visible);
}

void Application::SetVSync(bool enabled)
{
	glfwSwapInterval(enabled ? 1 : 0);
}

bool Application::HasWindowClosed()
{
	return glfwWindowShouldClose(window) == GLFW_TRUE;
}

unsigned int Application::GetWindowWidth() const
{
	int w = 0, h = 0;
	glfwGetWindowSize(window, &w, &h);
	return w;
}

unsigned int Application::GetWindowHeight() const
{
	int w = 0, h = 0;
	glfwGetWindowSize(window, &w, &h);
	return h;
}

float Application::GetTime() const
{
	return (float)glfwGetTime();
}

void Application::SetMousePosition(GLFWwindow* window, double x, double y)
{
	instance->mousePos.x = (float)x;
	instance->mousePos.y = (float)y;
}

bool Application::CreateWindow(const char* title, int width, int height, bool fullscreen)
{
	this->fullscreen = fullscreen;

	if (glfwInit() == false)
	{
		std::cout << "GLFW failed to initialise\n";
		return false;
	}

	window = glfwCreateWindow(width, height, title, (fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);

	if (!window)
	{
		std::cout << "Window creation failed\n";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		std::cout << "OpenGL failed to load\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	std::cout << "GL: " << GLVersion.major << "." << GLVersion.minor << '\n';

	glfwSetWindowSizeCallback(window, [](GLFWwindow*, int w, int h) 
		{ 
			glViewport(0, 0, w, h);
		});

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void Application::DestroyWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}