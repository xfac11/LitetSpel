#ifndef SELECT_GUI_H
#define SELECT_GUI_H

#include "GuiBase.h"
#include "GuiElement.h"
#include "Button.h"
#include "PlayerSelector.h"

class SelectGui : public GuiBase
{
private:
	GuiElement* selectedElement;
	Button* buttonStart;
	PlayerSelector* playerSelectors[4];

public:
	SelectGui(State* myState);
	virtual ~SelectGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !SELECT_GUI_H
