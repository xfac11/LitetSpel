#ifndef PAUSE_GUI_H
#define PAUSE_GUI_H

#include "GuiBase.h"
#include "GuiElement.h"
#include "Button.h"

class PauseGui : public GuiBase
{
private:
	GuiElement* selectedElement;
	Button* resumeButton;
	Button* mainMenuButton;

	float timeSinceChanged;
	bool changedLastFrame;

	void changeSelected_Keyboard();
	void changeSelected();
public:
	PauseGui(State* myState);
	virtual ~PauseGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !PAUSE_GUI_H
