#pragma once

#include "Event.h"

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(unsigned int width, unsigned int heigth)
		: width(width), height(height)
	{
	}

	unsigned int GetWidth() const { return width; }
	unsigned int GetHeight() const { return height; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << width << ", " << height;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowResize);
	EVENT_CLASS_CATEGORY(EventCategoryApplication);

private:
	unsigned int width, height;
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent()
	{
	}

	EVENT_CLASS_TYPE(WindowClose);
	EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

class WindowRefreshEvent : public Event
{
public:
	WindowRefreshEvent() {}

	EVENT_CLASS_TYPE(WindowRefresh);
	EVENT_CLASS_CATEGORY(EventCategoryApplication);
};