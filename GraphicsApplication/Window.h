#pragma once
#include "Events/Event.h"
#include "GraphicsContext.h"

#include <string>
#include <functional>

struct GLFWwindow;

struct WindowProperties
{
	std::string title;
	unsigned int width;
	unsigned int height;
	bool fullscreen;

	WindowProperties(const std::string& title = "Default Title", unsigned int width = 1280, unsigned int height = 720, bool fullscreen = false)
		: title(title), width(width), height(height), fullscreen(fullscreen) { }
};

class Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;

	Window(const WindowProperties properties);
	~Window();

	void Update();

	unsigned int GetWidth() const { return windowData.width; }
	unsigned int GetHeight() const { return windowData.height; }

	void SetEventCallback(const EventCallbackFn& callback) { windowData.eventCallback = callback; }
	void SetVSync(bool enabled);
	bool IsVSync() const;

	void* GetNativeWindow() const { return window; }

private:
	void Init(const WindowProperties& properties);
	void Shutdown();

private:
	GLFWwindow* window;
	GraphicsContext* context;

	struct WindowData
	{
		std::string title;
		unsigned int width, height;
		bool vsync;
		bool fullscreen;
		EventCallbackFn eventCallback;
	};

	WindowData windowData;
};