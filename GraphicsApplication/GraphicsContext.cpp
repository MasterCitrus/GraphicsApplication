#include "GraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

GraphicsContext::GraphicsContext(GLFWwindow* window) : window(window)
{

}

void GraphicsContext::Init()
{
	glfwMakeContextCurrent(window);
	if (!gladLoadGL())
	{
		std::cout << "OpenGL failed to load\n";
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}

void GraphicsContext::SwapBuffers()
{
	glfwSwapBuffers(window);
}
