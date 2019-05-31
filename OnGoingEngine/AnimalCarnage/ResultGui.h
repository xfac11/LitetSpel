#ifndef RESULT_GUI_H
#define RESULT_GUI_H

#include "GuiBase.h"
#include "Button.h"
#include "WinnerView.h"
#include "LooserView.h"

using namespace DirectX::SimpleMath;

class ResultGui : public GuiBase
{
private:
	Button* buttonMenu;
	LooserView* statsView[4];
	int nrOfPlayers;

	float buttonDelay;

public:
	ResultGui(State* myState);
	virtual ~ResultGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();

	void initializePlayerStats();
};

#endif // !RESULT_GUI_H
