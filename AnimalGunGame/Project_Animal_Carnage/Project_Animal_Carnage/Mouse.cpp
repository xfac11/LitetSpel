#include "Mouse.h"

Mouse::Mouse()
{
	bool leftIsDown = false;
	bool rightIsDown = false;
	bool middleIsDown = false;
	bool forwardIsDown = false;
	bool backIsDown = false;

	int x = 0;
	int y = 0;
}

Mouse::~Mouse()
{
}

void Mouse::OnLeftPressed(int x, int y)
{
	this->leftIsDown = true;
	this->eventBuffer.push(MouseEvent(MouseEventType::LPress, x, y));
}

void Mouse::OnLeftReleased(int x, int y)
{
	this->leftIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEventType::LRelease, x, y));
}

void Mouse::OnRightPressed(int x, int y)
{
	this->rightIsDown = true;
	this->eventBuffer.push(MouseEvent(MouseEventType::RPress, x, y));
}

void Mouse::OnRightReleased(int x, int y)
{
	this->rightIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEventType::RRelease, x, y));
}

void Mouse::OnMiddlePressed(int x, int y)
{
	this->middleIsDown = true;
	this->eventBuffer.push(MouseEvent(MouseEventType::MPress, x, y));
}

void Mouse::OnMiddleReleased(int x, int y)
{
	this->middleIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEventType::MRelease, x, y));
}

void Mouse::OnWheelUp(int x, int y)
{
	this->eventBuffer.push(MouseEvent(MouseEventType::WheelUp, x, y));
}

void Mouse::OnWheelDown(int x, int y)
{
	this->eventBuffer.push(MouseEvent(MouseEventType::WheelDown, x, y));
}

void Mouse::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
	this->eventBuffer.push(MouseEvent(MouseEventType::Move, x, y));
}

void Mouse::OnMouseRAW(int x, int y)
{
	this->eventBuffer.push(MouseEvent(MouseEventType::RAW_MOVE, x, y));
}

void Mouse::OnForwardPressed(int x, int y)
{
	this->forwardIsDown = true;
	this->eventBuffer.push(MouseEvent(MouseEventType::FPress, x, y));
}

void Mouse::OnForwardReleased(int x, int y)
{
	this->forwardIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEventType::FRelease, x, y));
}

void Mouse::OnBackPressed(int x, int y)
{
	this->backIsDown = true;
	this->eventBuffer.push(MouseEvent(MouseEventType::BPress, x, y));
}

void Mouse::OnBackReleased(int x, int y)
{
	this->backIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEventType::BRelease, x, y));
}

bool Mouse::IsLeftDown()
{
	return this->leftIsDown;
}

bool Mouse::IsRightDown()
{
	return this->rightIsDown;
}

bool Mouse::IsMiddleDown()
{
	return this->middleIsDown;
}

bool Mouse::IsForwardDown()
{
	return this->forwardIsDown;
}

bool Mouse::IsBackDown()
{
	return this->backIsDown;
}

int Mouse::GetPosX()
{
	return this->x;
}

int Mouse::GetPosY()
{
	return this->y;
}

MousePoint Mouse::GetPos()
{
	return { this->x, this->y };
}

bool Mouse::EventBufferIsEmpty()
{
	return this->eventBuffer.empty();
}

MouseEvent Mouse::ReadEvent()
{
	MouseEvent temp = MouseEvent();
	if (this->eventBuffer.empty()==false)
	{
		temp = this->eventBuffer.front();
		this->eventBuffer.pop();
	}
	return temp;
}
