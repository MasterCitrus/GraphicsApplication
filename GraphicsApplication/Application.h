#pragma once
#include "Camera.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include <glm/vec2.hpp>

struct GLFWwindow;

class Application
{
public:
	Application();
	virtual ~Application();

	void Run(const char* title, unsigned int width, unsigned int height, bool fullscreen);

	virtual bool Startup() = 0;
	virtual void Shutdown() = 0;
	virtual void Update(float delta) = 0;
	virtual void Draw() = 0;
	virtual void ImGuiDraw() {}

	void ClearScreen(glm::vec3 colour);
	void SetBackgroundColour(float r, float g, float b, float a = 1.0f);
	void Quit() { quit = true; }
	bool HasWindowClosed();

	virtual void OnEvent(Event& e);

	Window& GetWindow() const { return *window; }
	unsigned int GetFPS() const { return fps; }
	unsigned int GetWindowWidth() const;
	unsigned int GetWindowHeight() const;

	float GetTime() const;

	bool& GetFullscreen() { return fullscreen; }

	Camera* GetCamera() { return camera; }

	static Application* Get() { return instance; }

protected:
	virtual bool OnKeyPressed(KeyPressedEvent& e) { return false; }
	virtual bool OnMouseButtonPressed(MouseButtonPressedEvent& e) { return false; }

private:
	bool OnWindowClose(WindowCloseEvent& e);

protected:
	Camera* camera;

	static Application* instance;
	Window* window;
	unsigned int fps;
	bool quit;
	bool fullscreen;
	bool showDemoWindow = false;
};