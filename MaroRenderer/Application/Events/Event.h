#pragma once

enum class EventType
{
	KeyPress, KeyRelease, KeyTyped,
	WindowClose, WindowResize, WindowMove,
	MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
};

class  Event
{
public:

	bool m_Handled = false;

	virtual EventType GetType() const = 0;

};