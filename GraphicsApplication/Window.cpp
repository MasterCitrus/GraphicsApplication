#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#include <GLFW/glfw3.h>
#include <iostream>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>

Window::Window(const WindowProperties properties)
{
	Init(properties);
}

Window::~Window()
{
	Shutdown();
}

void Window::Update()
{
	glfwPollEvents();
	context->SwapBuffers();
}

void Window::SetVSync(bool enabled)
{
	if (enabled) glfwSwapInterval(1);
	else glfwSwapInterval(0);

	windowData.vsync = enabled;
}

bool Window::IsVSync() const
{
	return windowData.vsync;
}

void* Window::GetNativeWindowHandle() const
{
	return glfwGetWin32Window(window);
}

void Window::Init(const WindowProperties& properties)
{
	windowData.title = properties.title;
	windowData.width = properties.width;
	windowData.height = properties.height;
	windowData.fullscreen = properties.fullscreen;

	int success = glfwInit();
	if (!success)
	{
		std::cout << "GLFW failed to initialise.\n";
		__debugbreak;
	}

	window = glfwCreateWindow(windowData.width, windowData.height, windowData.title.c_str(), windowData.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

	if (!window)
	{
		std::cout << "Window failed to create.\n";
		__debugbreak;
	}

	context = new GraphicsContext(window);
	context->Init();

	glfwSetWindowUserPointer(window, &windowData);
	SetVSync(true);

	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
							  {
								  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
								  data.width = width;
								  data.height = height;

								  WindowResizeEvent event(width, height);
								  data.eventCallback(event);
							  });

	glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
							   {
								   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

								   WindowCloseEvent event;
								   data.eventCallback(event);
							   });

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
					   {
						   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

						   switch (action)
						   {
							   case GLFW_PRESS:
							   {
								   KeyPressedEvent event(key, false);
								   data.eventCallback(event);
								   break;
							   }
							   case GLFW_RELEASE:
							   {
								   KeyReleasedEvent event(key);
								   data.eventCallback(event);
								   break;
							   }
							   case GLFW_REPEAT:
							   {
								   KeyPressedEvent event(key, true);
								   data.eventCallback(event);
								   break;
							   }
						   }
					   });

	glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keycode)
						{
							WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

							KeyTypedEvent event(keycode);
							data.eventCallback(event);
						});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
							   {
								   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

								   switch (action)
								   {
									   case GLFW_PRESS:
									   {
										   MouseButtonPressedEvent event(button);
										   data.eventCallback(event);
										   break;
									   }
									   case GLFW_RELEASE:
									   {
										   MouseButtonReleasedEvent event(button);
										   data.eventCallback(event);
										   break;
									   }
								   }
							   });

	glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
						  {
							  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

							  MouseScrolledEvent event((float)xOffset, (float)yOffset);
							  data.eventCallback(event);
						  });

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
							 {
								 WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

								 MouseMovedEvent event((float)xPos, (float)yPos);
								 data.eventCallback(event);
							 });

	glfwSetWindowRefreshCallback(window, [](GLFWwindow* window)
								{
									WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

									WindowRefreshEvent event;
									data.eventCallback(event);
								});
}

void Window::Shutdown()
{
	glfwDestroyWindow(window);
	delete context;
}
