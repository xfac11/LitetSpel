#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H
struct MousePoint
{
	int x;
	int y;
};
enum MouseEventType
{
	LPress,
	LRelease,
	RPress,
	RRelease,
	MPress,
	MRelease,
	WheelUp,
	WheelDown,
	FPress,
	FRelease,
	BPress,
	BRelease,
	Move,
	RAW_MOVE,
	Wrong
};
class MouseEvent
{
private:
	MouseEventType type;
	int x;
	int y;
public:
	MouseEvent();
	MouseEvent(const MouseEventType type, const int x, const int y);
	virtual~MouseEvent();
	bool IsValid();
	MouseEventType GetType()const;
	MousePoint GetPos();
	int GetPosX()const;
	int GetPosY()const;
};
#endif