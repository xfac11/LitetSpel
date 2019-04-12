#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "State.h"

class MainMenu : public State
{
private:

public:
	MainMenu();
	~MainMenu();

	bool initailize();
	bool render();
	bool update(float deltaTime);
	void shutDown();
};

#endif // !MAIN_MENU_H