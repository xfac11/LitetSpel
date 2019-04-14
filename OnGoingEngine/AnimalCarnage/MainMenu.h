#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "State.h"
#include "SpriteBatch.h"
#include "Sprite.h"

class MainMenu : public State
{
private:
	DirectX::SpriteBatch* spritebatch;
	Sprite* sprite;
public:
	MainMenu();
	~MainMenu();

	bool initailize();
	bool render();
	bool update(float deltaTime);
	void shutDown();
};

#endif // !MAIN_MENU_H