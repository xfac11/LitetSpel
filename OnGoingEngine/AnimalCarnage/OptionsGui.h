#ifndef OPTIONS_GUI_H
#define OPTIONS_GUI_H

#include "GuiBase.h"
#include "GuiElement.h"
#include "Slider.h"

class OptionsGui : public GuiBase
{
private:
	GuiElement* selectedElement;
	Slider* volume;

	bool pressedLastFrame;

public:
	OptionsGui(State* myState);
	virtual ~OptionsGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !OPTIONS_GUI_H
