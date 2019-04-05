#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H
enum KeyboardEventType
{
	Press,
	Relsease,
	Invalid
};
class KeyboardEvent
{

private:
	KeyboardEventType type;
	unsigned char key;
public:

	KeyboardEvent();
	KeyboardEvent(const KeyboardEventType type, const unsigned char key);
	virtual~KeyboardEvent();
	bool IsPress()const;
	bool IsRelease() const;
	bool isValid() const;
	unsigned char GetKeyCode()const;
};
#endif