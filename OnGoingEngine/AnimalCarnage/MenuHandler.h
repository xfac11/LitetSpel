//this class only porpouse is to handle the menu item and its states
#ifndef MENUHANDLER_H
#define MENUHANDLER_H
#include "MenuItem.h"
class MenuHandler
{
private:
	
	int cap;
	MenuItem* *menus;
	
	int currentMenu;
	int currentOption;

public:
	MenuHandler();
	~MenuHandler();

	void optionSelection(int i); //expects -1, 0 or 1
	void confirm();
	void back();
	void reset();

	std::string getCurrentMenu();
	int getCurrentOption();
};
#endif
