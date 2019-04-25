#include "GuiBase.h"

GuiBase::GuiBase(State * myState) : myState(myState)
{
	this->keyboardDelay = 0.0F;
}

GuiBase::~GuiBase()
{
}

void GuiBase::activateDelay()
{
	this->keyboardDelay = 0.2F;
}
