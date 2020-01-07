#pragma once

#include "Event.h"

class MouseMovedEvent : public Event
{
public:

	MouseMovedEvent(int xPos, int yPos) : m_xPos(xPos), m_yPos(yPos) {}

	inline const int GetX() { return m_xPos; }
	inline const int GetY() { return m_yPos; }

	virtual EventType GetType() const override { return EventType::MouseMove; }

private:

	int m_xPos, m_yPos;
};

class MousePressedEvent : public Event
{
public:

	MousePressedEvent(int button, int xPos, int yPos) : m_Button(button), m_xPos(xPos), m_yPos(yPos) {}

	virtual EventType GetType() const override { return EventType::MouseButtonPress; }
	inline const int GetButton() { return m_Button; }
	inline const int GetX() { return m_xPos; }
	inline const int GetY() { return m_yPos; }

private:

	int m_Button;
	int m_xPos, m_yPos;
};

class MouseReleasedEvent : public Event
{
public:

	MouseReleasedEvent(int button, int xPos, int yPos) : m_Button(button), m_xPos(xPos), m_yPos(yPos) {}

	virtual EventType GetType() const override { return EventType::MouseButtonRelease; }
	inline const int GetButton() { return m_Button; }
	inline const int GetX() { return m_xPos; }
	inline const int GetY() { return m_yPos; }

private:

	int m_Button;
	int m_xPos, m_yPos;

};
