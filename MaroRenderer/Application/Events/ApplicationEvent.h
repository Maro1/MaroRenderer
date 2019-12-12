#pragma once

#include "Event.h"

class  WindowResizeEvent : public Event {

public:
	WindowResizeEvent(unsigned int Width, unsigned int Height) : m_Width(Width),
		m_Height(Height) {}

	unsigned int GetWidth() const { return m_Width; }
	unsigned int GetHeight() const { return m_Height; }

	virtual EventType GetType() const override { return EventType::WindowResize; }

private:
	unsigned int m_Width;
	unsigned int m_Height;

};

class  WindowCloseEvent : public Event {

public:
	WindowCloseEvent() {}

	virtual EventType GetType() const override { return EventType::WindowClose; }
};