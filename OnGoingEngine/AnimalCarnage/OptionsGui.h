#ifndef OPTIONS_GUI_H
#define OPTIONS_GUI_H

#include "GuiBase.h"
#include "GuiElement.h"
#include "Slider.h"
#include "CheckBox.h"

class OptionsGui : public GuiBase
{
private:
	GuiElement* selectedElement;
	Slider* sliderMusic;
	Slider* sliderSound;
	CheckBox* checkBoxRumble1;
	CheckBox* checkBoxRumble2;
	CheckBox* checkBoxRumble3;
	CheckBox* checkBoxRumble4;

	bool pressedLastFrame;
	bool changedLastFrame;
	float timeSinceChanged;

	void changeSelected_Keyboard();
	void changeSelected();

public:
	OptionsGui(State* myState);
	virtual ~OptionsGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !OPTIONS_GUI_H
