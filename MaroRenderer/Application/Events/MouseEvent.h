#pragma once

#include "Event.h"

class MouseMovedEvent : public Event
{
public:

	MouseMovedEvent(int xpos, int ypos) : m_xPos(xpos), m_yPos(ypos) {}

	inline const int GetX() { return m_xPos; }
	inline const int GetY() { return m_yPos; }

	virtual EventType GetType() const override { return EventType::MouseMove; }

private:

	int m_xPos, m_yPos;
};

class MousePressedEvent : public Event
{
public:

	MousePressedEvent(int button) : m_Button(button) {}

	virtual EventType GetType() const override { return EventType::MouseButtonPress; }
	inline int GetButton() { return m_Button; }

private:

	int m_Button;
};

class MouseReleasedEvent : public Event
{
public:

	MouseReleasedEvent(int button) : m_Button(button) {}

	virtual EventType GetType() const override { return EventType::MouseButtonRelease; }
	inline int GetButton() { return m_Button; }

private:

	int m_Button;

};
