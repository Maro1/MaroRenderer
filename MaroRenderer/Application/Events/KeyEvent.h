#pragma once

#include "Event.h"

class KeyEvent : public Event
{
public:
	int GetKeyCode() const { return m_Keycode; }

protected:
	int m_Keycode;
	KeyEvent(int Keycode) : m_Keycode(Keycode) { }
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int Keycode, int RepeatCount) : KeyEvent(Keycode),
		m_RepeatCount(RepeatCount) {}

	virtual EventType GetType() const override { return EventType::KeyPress; }

private:
	int m_RepeatCount;

};

class  KeyReleasedEvent : public KeyEvent {

public:
	KeyReleasedEvent(int Keycode) : KeyEvent(Keycode) {}

	virtual EventType GetType() const override { return EventType::KeyRelease; }

};

class  KeyTypedEvent : public KeyEvent {

public:
	KeyTypedEvent(int Keycode) : KeyEvent(Keycode) {}

	virtual EventType GetType() const override { return EventType::KeyTyped; }

};