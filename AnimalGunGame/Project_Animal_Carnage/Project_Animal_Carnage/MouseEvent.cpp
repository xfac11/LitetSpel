#include "MouseEvent.h"

MouseEvent::MouseEvent()
{
	this->type = MouseEventType::Wrong;
	this->x = 0;
	this->y = 0;
}

MouseEvent::MouseEvent(const MouseEventType type, const int x, const int y)
{
	this->type = type;
	this->x = x;
	this->y = y;
}

MouseEvent::~MouseEvent()
{
}

bool MouseEvent::IsValid()
{
	return this->type != MouseEventType::Wrong;
}

MouseEventType MouseEvent::GetType() const
{
	return this->type;
}

MousePoint MouseEvent::GetPos()
{

	return { this->x, this->y };
}

int MouseEvent::GetPosX() const
{
	return this->x;
}

int MouseEvent::GetPosY() const
{
	return this->y;
}
