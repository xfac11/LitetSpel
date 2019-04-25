#ifndef SELECT_GUI_H
#define SELECT_GUI_H

#include "GuiBase.h"
#include "GuiElement.h"
#include "Button.h"

class SelectGui : public GuiBase
{
private:
	GuiElement* selectedElement;
	Button* buttonTest;

	bool changedLastFrame;
	float timeSinceChanged;

	void changeSelected_Keyboard();
	void changeSelected();

public:
	SelectGui(State* myState);
	virtual ~SelectGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !SELECT_GUI_H
