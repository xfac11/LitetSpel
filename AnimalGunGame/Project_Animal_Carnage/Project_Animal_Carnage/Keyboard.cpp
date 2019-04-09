#include "Keyboard.h"

void Keyboard::EnableAutoRepeatKeys()
{
	this->autoRepeatKeys = true;
}

void Keyboard::DisableAutoRepeatKeys()
{
	this->autoRepeatKeys = false;
}

void Keyboard::EnableAutoRepeatChars()
{
	this->autoRepeatChars = true;
}

void Keyboard::DisableAutoRepeatChars()
{
	this->autoRepeatChars = false;
}

bool Keyboard::IsKeysAutoRepeat()
{
	return this->autoRepeatKeys;
}

bool Keyboard::IsCharsAutoRepeat()
{
	return this->autoRepeatChars;
}

Keyboard::Keyboard()
{
	for (int i = 0; i < 256; i++)
	{
		this->KeyStates[i] = false;
	}
	this->autoRepeatKeys = false;
	this->autoRepeatChars = false;
}

Keyboard::~Keyboard()
{
}

bool Keyboard::KeyIsPressed(const unsigned char keycode)
{
	return this->KeyStates[keycode];
}

bool Keyboard::KeyBufferIsEmpty()
{
	return this->keyBuffer.empty(); // if()
}

bool Keyboard::CharBufferIsEmpty()
{
	return this->charBuffer.empty();
}

KeyboardEvent Keyboard::ReadKey()
{
	KeyboardEvent temp = KeyboardEvent();

	if (this->keyBuffer.empty() == false)
	{
		temp = this->keyBuffer.front();
		this->keyBuffer.pop();
	}
	return temp;
}

unsigned char Keyboard::ReadChar()
{
	char temp = 0u;

	if (this->charBuffer.empty() == false)
	{
		temp = this->charBuffer.front();
		this->charBuffer.pop();
	}
	return temp;
}

void Keyboard::OnKeyPressed(const unsigned char key)
{
	this->KeyStates[key] = true;
	this->keyBuffer.push(KeyboardEvent(KeyboardEventType::Press, key));
}

void Keyboard::OnKeyReleased(const unsigned char key)
{
	this->KeyStates[key] = false;
	this->keyBuffer.push(KeyboardEvent(KeyboardEventType::Relsease, key));
}

void Keyboard::OnChar(const unsigned char key)
{
	this->charBuffer.push(key);
}
