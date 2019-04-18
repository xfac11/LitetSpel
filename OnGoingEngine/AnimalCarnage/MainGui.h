#ifndef MAIN_GUI_H
#define MAIN_GUI_H

#include "GuiBase.h"
#include "Button.h"
#include "GamePad.h"

class MainGui : public GuiBase
{
private:
	//rumble
	struct RumbleTemp
	{
		float rumbleClock;
		float rumbleTime;
		DirectX::XMFLOAT2 rumble;
	};
	RumbleTemp theRumble[4];

	GuiElement* selectedElement;
	Button* playButton;
	Button* quitButton;

	//float timeSinceChanged;
	bool changedLastFrame;
	void changeSelected(int i);

	void updateRumble(float deltaTime, int id);

public:
	MainGui(State* myStae);
	virtual ~MainGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();

	bool reset(DirectX::GamePad::State state);
};

#endif // !MAIN_GUI_H


