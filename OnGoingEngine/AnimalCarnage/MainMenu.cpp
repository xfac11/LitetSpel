#include "MainMenu.h"

MainMenu::MainMenu()
{
	this->sprites = new Sprite("cat.tga",0.1f);
}

MainMenu::~MainMenu()
{
	delete this->sprites;
}

bool MainMenu::initailize()
{
	return true;
}

bool MainMenu::render()
{
	sprites->Render();
	return true;
}

bool MainMenu::update(float deltaTime)
{

	return true;
}

void MainMenu::shutDown()
{
}
