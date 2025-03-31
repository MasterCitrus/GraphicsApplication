#pragma once

#include "Event.h"

class KeyEvent : public Event
{
public:
	int GetKeyCode() const { return keycode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);

protected:
	KeyEvent(int keycode) : keycode(keycode) {}

	int keycode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int keycode, bool isRepeat = false) : KeyEvent(keycode), isRepeat(isRepeat) {}

	bool IsRepeat() const { return isRepeat; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << keycode << " (repeat = " << isRepeat << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed);

private:
	bool isRepeat;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << keycode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased);
};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << keycode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyTyped);
};