#ifndef MAIN_GUI_H
#define MAIN_GUI_H

#include "GuiBase.h"
#include "Button.h"

class MainGui : public GuiBase
{
private:
	GuiElement* selectedElement;
	Button* playButton;
	Button* quitButton;

	float timeSinceChanged;
	bool changedLastFrame;
	void changeSelected();

public:
	MainGui();
	virtual ~MainGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !MAIN_GUI_H


