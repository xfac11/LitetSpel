#include "MainMenu.h"
#include "MainGui.h"

MainMenu::MainMenu()
{
	this->gui = nullptr;
}

MainMenu::~MainMenu()
{
	delete this->gui;
}

bool MainMenu::initailize()
{
	this->gui = new MainGui();
	this->gui->initialize();

	return true;
}

bool MainMenu::render()
{
	this->gui->render();
	return true;
}

bool MainMenu::update(float deltaTime)
{
	this->gui->update(deltaTime);
	return true;
}

void MainMenu::shutDown()
{
	delete this->gui;
}
