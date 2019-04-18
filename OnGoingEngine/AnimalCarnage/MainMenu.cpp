#include "MainMenu.h"
#include "MainGui.h"

MainMenu::MainMenu()
{
	this->main = nullptr;
	this->options = nullptr;
}

MainMenu::~MainMenu()
{
	delete this->main;
	delete this->options;
}

bool MainMenu::initailize()
{
	this->main = new MainGui();
	this->options = new MainGui();
	this->main->initialize();
	this->options->initialize();

	this->gui = main;

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
	delete this->main;
	delete this->options;
}

void MainMenu::setCurrentMenu(MainMenuGui menu)
{
	switch (menu)
	{
	case MAIN:
		this->gui = main;
		break;
	case OPTIONS:
		this->gui = options;
		break;
	}
}
