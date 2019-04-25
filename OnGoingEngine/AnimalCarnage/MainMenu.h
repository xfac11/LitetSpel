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

public:
	MainMenu();
	~MainMenu();

	bool initailize();
	bool render();
	bool update(float deltaTime);
	void shutDown();

	void setCurrentMenu(MainMenuGui menu);
};

#endif // !MAIN_MENU_H