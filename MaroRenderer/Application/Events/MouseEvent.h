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

	MousePressedEvent(int button, int xPos, int yPos, int mods) : m_Button(button), m_xPos(xPos), m_yPos(yPos), m_Mods(mods) {}

	virtual EventType GetType() const override { return EventType::MouseButtonPress; }
	inline int GetButton() const { return m_Button; }
	inline int GetX() const { return m_xPos; }
	inline int GetY() const { return m_yPos; }
	inline int GetMods() const { return m_Mods; }

private:

	int m_Button;
	int m_xPos, m_yPos;
	int m_Mods;
};

class MouseReleasedEvent : public Event
{
public:

	MouseReleasedEvent(int button, int xPos, int yPos) : m_Button(button), m_xPos(xPos), m_yPos(yPos) {}

	virtual EventType GetType() const override { return EventType::MouseButtonRelease; }
	inline int GetButton() const { return m_Button; }
	inline int GetX() const { return m_xPos; }
	inline int GetY() const { return m_yPos; }

private:

	int m_Button;
	int m_xPos, m_yPos;

};

class MouseScrolledEvent : public Event
{
public:

	MouseScrolledEvent(int xOffset, int yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}

	virtual EventType GetType() const override { return EventType::MouseScroll; }
	inline int GetOffsetX() const { return m_xOffset; }
	inline int GetOffsetY() const { return m_yOffset; }

private:
	int m_xOffset, m_yOffset;
};
