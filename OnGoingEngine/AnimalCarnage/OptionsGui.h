#ifndef OPTIONS_GUI_H
#define OPTIONS_GUI_H

#include "GuiBase.h"
#include "GuiElement.h"
#include "Slider.h"
#include "CheckBox.h"
#include "Button.h"

enum Resolution;

class OptionsGui : public GuiBase
{
private:
	GuiElement* selectedElement;
	Slider* sliderMusic;
	Button* buttonResolution;
	Resolution resolution;
	Button* buttonConfirm;
	CheckBox* checkBoxRumble1;
	CheckBox* checkBoxRumble2;
	CheckBox* checkBoxRumble3;
	CheckBox* checkBoxRumble4;

	bool sliderSelected;
	bool pressedLastFrame;
	bool changedLastFrame;
	float timeSinceChanged;

	void changeSelected_Keyboard();
	void changeSelected();

	std::string getResolutionText(Resolution res);
	void setResolution();
	void changeResolution();
public:
	OptionsGui(State* myState);
	virtual ~OptionsGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !OPTIONS_GUI_H
