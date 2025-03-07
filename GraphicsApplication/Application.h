#pragma once
#include "Camera.h"
#include <glm/vec2.hpp>

struct GLFWwindow;

class Application
{
public:
	Application();
	virtual ~Application();

	void Run(const char* title, int width, int height, bool fullscreen);

	virtual bool Startup() = 0;
	virtual void Shutdown() = 0;
	virtual void Update(float delta) = 0;
	virtual void Draw() = 0;

	void ClearScreen();
	void SetBackgroundColour(float r, float g, float b, float a = 1.0f);
	void SetShowCursor(bool visible);
	void SetVSync(bool enabled);
	void Quit() { quit = true; }
	bool HasWindowClosed();


	GLFWwindow* GetWindowPtr() const { return window; }
	unsigned int GetFPS() const { return fps; }
	unsigned int GetWindowWidth() const;
	unsigned int GetWindowHeight() const;

	float GetTime() const;

	glm::vec2 GetMousePosition() { return mousePos; }
	glm::vec2 GetMouseDelta() { return mousePos - lastMousePos; }

	static Application* Get() { return instance; }
	static void SetMousePosition(GLFWwindow* window, double x, double y);

protected:
	virtual bool CreateWindow(const char* title, int width, int height, bool fullscreen);
	virtual void DestroyWindow();

	Camera camera;

	glm::vec2 mousePos;
	glm::vec2 lastMousePos;
	static Application* instance;
	GLFWwindow* window;
	unsigned int fps;
	bool quit;
};