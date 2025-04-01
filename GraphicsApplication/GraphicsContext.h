#pragma once

struct GLFWwindow;

class GraphicsContext
{
public:
	GraphicsContext(GLFWwindow* window);

	void Init();
	void SwapBuffers();

private:
	GLFWwindow* window;
};