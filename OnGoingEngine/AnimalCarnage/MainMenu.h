#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "State.h"

enum MainMenuGui { MAIN, OPTIONS, RULES, SELECT };

class MainMenu : public State
{
private:
	GuiBase* main;
	GuiBase* options;
	GuiBase* rules;
	GuiBase* select;

	bool rumbleEnabled[4];

public:
	MainMenu();
	~MainMenu();

	bool initailize();
	bool render();
	void renderImgui();
	bool update(float deltaTime);
	void shutDown();

	void setCurrentMenu(MainMenuGui menu, bool alwaysActivateDelay = false);

	void getRumble(bool rumbleEnabled[]);
	void setRumble(bool rumbleEnabled[]);
};

#endif // !MAIN_MENU_H