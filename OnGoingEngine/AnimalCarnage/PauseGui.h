#ifndef PAUSE_GUI_H
#define PAUSE_GUI_H

#include "GuiBase.h"
#include "GuiElement.h"
#include "Button.h"
#include "GamePad.h"

class PauseGui : public GuiBase
{
private:
	GuiElement* selectedElement;
	Button* resumeButton;
	Button* hideGUIButton;
	Button* mainMenuButton;

	bool showGui;
	float timeSinceChanged;
	bool changedLastFrame;

	void changeSelected_Keyboard();
	void changeSelected();
	bool checkReset(DirectX::GamePad::State state);
public:
	PauseGui(State* myState);
	virtual ~PauseGui();

	//void changeCamera(DirectX::XMFLOAT3 & camera);
	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !PAUSE_GUI_H
