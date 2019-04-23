#ifndef RULES_GUI_H
#define RULES_GUI_H

#include "GuiBase.h"
#include "GuiElement.h"
#include "Button.h"
#include "CheckBox.h"

class RulesGui : public GuiBase
{
private:
	GuiElement* selectedElement;
	CheckBox* testCheckBox;
	Button* confirmButton;

	void changeSelected();

public:
	RulesGui(State* myState);
	virtual ~RulesGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !RULES_GUI_H
