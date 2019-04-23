#include "MainMenu.h"
#include "MainGui.h"
#include "OptionsGui.h"
#include "RulesGui.h"

MainMenu::MainMenu()
{
	this->main = nullptr;
	this->options = nullptr;
}

MainMenu::~MainMenu()
{
	delete this->main;
	delete this->options;
	delete this->rules;
}

bool MainMenu::initailize()
{
	this->main = new MainGui(this);
	this->options = new OptionsGui(this);
	this->rules = new RulesGui(this);
	this->main->initialize();
	this->options->initialize();
	this->rules->initialize();

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
	delete this->rules;
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
	case RULES:
		this->gui = rules;
		break;
	}
}
