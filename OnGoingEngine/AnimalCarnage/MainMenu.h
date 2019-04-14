#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "State.h"
#include "Sprite.h"

class MainMenu : public State
{
private:
	Sprite* sprites;
public:
	MainMenu();
	~MainMenu();

	bool initailize();
	bool render();
	bool update(float deltaTime);
	void shutDown();
};

#endif // !MAIN_MENU_H