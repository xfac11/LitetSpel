#include "PauseGui.h"
#include "System.h"
#include "GunGameState.h"

void PauseGui::changeSelected_Keyboard()
{
	GuiElement* newSelected = nullptr;

	if (System::theKeyboard->KeyIsPressed('W'))
	{
		newSelected = this->selectedElement->getUp();
	}
	if (System::theKeyboard->KeyIsPressed('S'))
	{
		newSelected = this->selectedElement->getDown();
	}
	if (System::theKeyboard->KeyIsPressed('A'))
	{
		newSelected = this->selectedElement->getLeft();
	}
	if (System::theKeyboard->KeyIsPressed('D'))
	{
		newSelected = this->selectedElement->getRight();
	}

	if (newSelected != nullptr)
	{
		this->changedLastFrame = true;
		this->selectedElement = newSelected;
	}
	else
	{
		this->changedLastFrame = false;
	}
}

void PauseGui::changeSelected()
{
	GuiElement* newSelected = nullptr;

	if (System::theTracker->dpadUp == DirectX::GamePad::ButtonStateTracker::PRESSED)
	{
		newSelected = this->selectedElement->getUp();
	}
	if (System::theTracker->dpadDown == DirectX::GamePad::ButtonStateTracker::PRESSED)
	{
		newSelected = this->selectedElement->getDown();
	}
	if (System::theTracker->dpadLeft == DirectX::GamePad::ButtonStateTracker::PRESSED)
	{
		newSelected = this->selectedElement->getLeft();
	}
	if (System::theTracker->dpadRight == DirectX::GamePad::ButtonStateTracker::PRESSED)
	{
		newSelected = this->selectedElement->getRight();
	}

	if (newSelected != nullptr)
	{
		this->selectedElement = newSelected;
	}
}

bool PauseGui::checkReset(DirectX::GamePad::State state)
{
	bool result = false;
	//if (state.IsConnected())
	//{
		//tracker.Update(state);
	if (((state.IsLeftTriggerPressed() && state.IsRightTriggerPressed()) ||
		(state.buttons.leftShoulder && state.buttons.rightShoulder)) &&
		state.buttons.a && (state.buttons.back || state.buttons.menu))
	{
		result = true;
	}
	//}
	return result;
}



PauseGui::PauseGui(State * myState) : GuiBase(myState)
{
	this->selectedElement = nullptr;

	this->resumeButton = nullptr;
	this->mainMenuButton = nullptr;

	this->timeSinceChanged = 0.0F;
	this->changedLastFrame = false;
}

PauseGui::~PauseGui()
{
	delete this->resumeButton;
	delete this->mainMenuButton;
}

//void PauseGui::changeCamera(DirectX::XMFLOAT3 & camera)
//{
//
//
////	//		//tracker.Update(state);
//////	if (((state.IsLeftTriggerPressed() && state.IsRightTriggerPressed()) ||
//////		(state.buttons.leftShoulder && state.buttons.rightShoulder)) &&
//////		state.buttons.a && (state.buttons.back || state.buttons.menu))
//////	{
////	DirectX::GamePad::State state = System::theGamePad->GetState(0);
////	float dirX = 17.0f * state.thumbSticks.leftX;
////	float dirY = 17.0f * state.thumbSticks.leftY;
////
////	camera.x += dirX;
////	camera.y += dirY;
//}

bool PauseGui::initialize()
{
	this->resumeButton = new Button("Resume", Vector2(System::theWindow.width / 2.0F - 300, System::theWindow.height / 2.0F - 140));
	this->mainMenuButton = new Button("Main Menu", Vector2(System::theWindow.width / 2.0F - 300, System::theWindow.height / 2.0F));

	this->selectedElement = resumeButton;
	this->resumeButton->setConnectedElements(nullptr, nullptr, mainMenuButton, mainMenuButton);
	this->mainMenuButton->setConnectedElements(nullptr, nullptr, resumeButton, resumeButton);

	return true;
}

void PauseGui::shutDown()
{
	delete this->resumeButton;
	delete this->mainMenuButton;

	this->selectedElement = nullptr;

	this->resumeButton = nullptr;
	this->mainMenuButton = nullptr;

	this->timeSinceChanged = 0.0F;
	this->changedLastFrame = false;
}

bool PauseGui::update(float deltaTime)
{
	GunGameState* state = dynamic_cast<GunGameState*>(this->myState);

	if (this->keyboardDelay <= 0.0F)
	{
		if (this->changedLastFrame)
		{
			if (this->timeSinceChanged > 0.2F)
			{
				this->timeSinceChanged -= 0.2F;
				this->changeSelected_Keyboard();
			}

			this->timeSinceChanged += deltaTime;
		}
		else
		{
			this->timeSinceChanged = 0.0F;
			this->changeSelected_Keyboard();
		}

		if (System::theKeyboard->KeyIsPressed('E'))
		{
			if (this->selectedElement == this->resumeButton)
			{
				state->pause(false);
			}
			else
			{
				state->pause(false);
				System::setState(MAINMENU);
			}
		}
	}
	else
	{
		this->keyboardDelay -= deltaTime;
	}

	for (int i = 0; i < 4; i++)
	{
		DirectX::GamePad::State gamepadState = System::theGamePad->GetState(i);

		if (gamepadState.IsConnected())
		{
			System::theTracker->Update(gamepadState);
			this->changeSelected();

			if (checkReset(gamepadState) == true) //L + R + A + Start
			{
				state->pause(false);
				System::setState(MAINMENU);
			}

			if (System::theTracker->start == DirectX::GamePad::ButtonStateTracker::PRESSED) // start
			{
				state->pause(false);
			}

			if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				if (this->selectedElement == this->resumeButton)
				{
					state->pause(false);
				}
				else
				{
					state->pause(false);
					System::setState(MAINMENU);
				}
			}

			break;
		}
	}

	return true;
}

bool PauseGui::render()
{
	System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied());

	System::getFontArial()->DrawString(System::getSpriteBatch(), "Game Paused", SimpleMath::Vector2(System::theWindow.width / 2.0F - SimpleMath::Vector2(System::getFontArial()->MeasureString("Game Paused")).x / 2, System::theWindow.height / 2.0F - 300), DirectX::Colors::Black, 0.0f, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2::One);
	this->resumeButton->render(this->selectedElement == resumeButton);
	this->mainMenuButton->render(this->selectedElement == mainMenuButton);

	System::getSpriteBatch()->End();
	return true;
}
