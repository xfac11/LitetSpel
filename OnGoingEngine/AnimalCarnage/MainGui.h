#ifndef MAIN_GUI_H
#define MAIN_GUI_H

#include "CommonStates.h"
#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "GuiBase.h"
#include "Button.h"

class MainGui : public GuiBase
{
private:
	DirectX::CommonStates* commonStates;
	DirectX::SpriteBatch* spriteBatch;
	DirectX::SpriteFont* spriteFont;
	
	GuiElement* selectedElement;
	Button* playButton;
	Button* quitButton;

public:
	MainGui();
	virtual ~MainGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !MAIN_GUI_H


