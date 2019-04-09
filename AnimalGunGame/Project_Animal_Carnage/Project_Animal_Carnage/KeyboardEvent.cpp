#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent()
{
	this->type = Invalid;
	this->key = 0u;
} 

KeyboardEvent::KeyboardEvent(const KeyboardEventType type, const unsigned char key)
{
	this->type = type;
	this->key = key;
}

KeyboardEvent::~KeyboardEvent()
{
}

bool KeyboardEvent::IsPress() const
{
	return this->type == Press;
}

bool KeyboardEvent::IsRelease() const
{
	return this->type == Relsease;
}

bool KeyboardEvent::isValid() const
{
	return this->type != Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
	return this->key;
}
