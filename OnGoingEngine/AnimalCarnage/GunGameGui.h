#ifndef GUN_GAME_GUI_H
#define GUN_GAME_GUI_H

#include "GuiBase.h"
#include "HealthBar.h"

class GunGameGui : public GuiBase
{
private:
	int nrOfPlayers;
	HealthBar** playerHealthBars;

	//bool pauseSwitch;
	//void change(bool & theSwitch);
public:
	GunGameGui(State* myState);
	virtual ~GunGameGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !GUN_GAME_GUI_H
