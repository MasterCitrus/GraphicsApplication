#pragma once

#include "Event.h"

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(float x, float y) : mouseX(x), mouseY(y) {}

	float GetX() const { return mouseX; }
	float GetY() const { return mouseY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << mouseX << ", " << mouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved);
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

private:
	float mouseX, mouseY;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(float xOffset, float yOffset) : xOffset(xOffset), yOffset(yOffset) {}

	float GetXOffset() const { return xOffset; }
	float GetYOffset() const { return yOffset; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << xOffset << ", " << yOffset;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled);
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

private:
	float xOffset, yOffset;
};

class MouseButtonEvent : public Event
{
public:
	int GetMouseButton() const { return button; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

protected:
	MouseButtonEvent(int button) : button(button) {}

	int button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed);
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased);
};