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
	Button* buttonGunGame;
	Button* buttonVersus;
	Button* buttonFalling;
	Button* buttonTagTeam;

	bool changedLastFrame;
	float timeSinceChanged;

	void changeSelected_Keyboard();
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
