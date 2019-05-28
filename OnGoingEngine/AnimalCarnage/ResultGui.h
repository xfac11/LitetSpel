#ifndef RESULT_GUI_H
#define RESULT_GUI_H

#include "GuiBase.h"
#include "Button.h"
#include "WinnerView.h"
#include "LooserView.h"

struct PlayerResultInfo
{
	int playerID;
	int kills;
	int deaths;
	int damage;
};

using namespace DirectX::SimpleMath;

class ResultGui : public GuiBase
{
private:
	Button* buttonMenu;
	WinnerView* winnerView;
	LooserView* looserView[3];

	float buttonDelay;

public:
	ResultGui(State* myState);
	virtual ~ResultGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !RESULT_GUI_H
