#ifndef MOUSE_H
#define MOUSE_H
#include "MouseEvent.h"
#include <queue>
class Mouse
{

private:
	std::queue<MouseEvent> eventBuffer;
	bool leftIsDown;// = false;
	bool rightIsDown; //= false;
	bool middleIsDown; //=false;
	bool forwardIsDown; 
	bool backIsDown;

	int x;
	int y;
public:
	Mouse();
	virtual~Mouse();
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseRAW(int x, int y);

	void OnForwardPressed(int x, int y);
	void OnForwardReleased(int x, int y);
	void OnBackPressed(int x, int y);
	void OnBackReleased(int x, int y);

	bool IsLeftDown();
	bool IsRightDown();
	bool IsMiddleDown();
	bool IsForwardDown();
	bool IsBackDown();

	int GetPosX();
	int GetPosY();
	MousePoint GetPos();

	bool EventBufferIsEmpty();
	MouseEvent ReadEvent();


};
#endif