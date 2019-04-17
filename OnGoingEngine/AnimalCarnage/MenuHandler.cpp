#include "MenuHandler.h"

MenuHandler::MenuHandler()
{
	this->cap = 8;
	this->menus = new MenuItem*[cap];

	//start screen, 0
	this->menus[0] = new MenuItem("Start", 0, 1); //name, id, nr of options
	this->menus[0]->setPrev(-1); //prev is disabled
	this->menus[0]->setNext(0,1); //to main menu

	//main menu, 1
	this->menus[1] = new MenuItem("Main_Menu", 1, 3);
	this->menus[1]->setPrev(0); //prev is start screen
	this->menus[1]->setNext(0, 2); //to game rules
	this->menus[1]->setNext(1, 3); //options
	this->menus[1]->setNext(2, 4); //exit

	//game rules, 2
	this->menus[2] = new MenuItem("Game_Rules", 2, 1); //name, id, nr of options
	this->menus[2]->setPrev(1); //prev is main menu
	this->menus[2]->setNext(0, 5); //to character select

	//options, 3
	this->menus[3] = new MenuItem("Options", 3, 1); //name, id, nr of options
	this->menus[3]->setPrev(1); //prev is main menu
	//this->menus[3]->setNext(0, 5); //

	//exit, 4
	this->menus[4] = new MenuItem("Exit", 4, 1); //name, id, nr of options
	this->menus[4]->setPrev(0); //prev is main menu

	//character Select, 5 
	this->menus[5] = new MenuItem("Character_Select", 5, 1); //name, id, nr of options
	this->menus[5]->setPrev(2); //prev is main menu
	this->menus[5]->setNext(0, 6); //to game

	//in game, 6
	this->menus[6] = new MenuItem("In_Game", 6, 1); //name, id, nr of options
	this->menus[6]->setPrev(2); //prev is main menu
	this->menus[6]->setNext(0, 7); //to result

	//result screen, 7
	this->menus[7] = new MenuItem("Character_Select", 7, 1); //name, id, nr of options
	this->menus[7]->setPrev(-1); //prev is disabled
	this->menus[7]->setNext(0, 5); //to character select
}

MenuHandler::~MenuHandler()
{
	for(int i=0;i<cap;i++)
	{
		delete menus[i];

	}
	delete[] menus;
}

void MenuHandler::optionSelection(int i)
{
	this->currentOption += i;
	if (this->currentOption >= menus[currentMenu]->getNrOfOptions())
	{
		this->currentOption = 0;
	}
	else if (this->currentOption < 0)
	{
		this->currentOption = menus[currentMenu]->getNrOfOptions() - 1;
	}
}

void MenuHandler::confirm()
{
	if (menus[currentMenu]->getNext(currentOption) != -1)
	{
		currentMenu = menus[currentMenu]->getNext(currentOption);
		currentOption = 0;
	}
}

void MenuHandler::back()
{
	if (menus[currentMenu]->getPrev() != -1)
	{
		currentMenu = menus[currentMenu]->getPrev();
		currentOption = 0;
	}
}

void MenuHandler::reset()
{
	this->currentMenu = 0;
	this->currentOption = 0;
}

std::string MenuHandler::getCurrentMenu()
{
	return menus[currentMenu]->getName();
}

int MenuHandler::getCurrentOption()
{
	return currentOption;
}
