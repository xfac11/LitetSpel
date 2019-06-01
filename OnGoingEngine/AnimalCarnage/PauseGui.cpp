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
	if (this->showGui == true)
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
	this->hideGUIButton = nullptr;
	this->mainMenuButton = nullptr;

	this->showGui = true;
	this->timeSinceChanged = 0.0F;
	this->changedLastFrame = false;
}

PauseGui::~PauseGui()
{
	delete this->resumeButton;
	delete this->hideGUIButton;
	delete this->mainMenuButton;
}

bool PauseGui::initialize()
{
	this->resumeButton	 =	new Button("Resume",		Vector2(1920 / 2.0F - 300, 1080 / 2.0F - 280));
	this->hideGUIButton  =	new Button("Toggle GUI",	Vector2(1920 / 2.0F - 300, 1080 / 2.0F - 140));
	this->mainMenuButton =	new Button("Main Menu",		Vector2(1920 / 2.0F - 300, 1080 / 2.0F));

	this->selectedElement = resumeButton;
	this->resumeButton->setConnectedElements(nullptr, nullptr, mainMenuButton, hideGUIButton);
	this->hideGUIButton->setConnectedElements(nullptr, nullptr, resumeButton, mainMenuButton);
	this->mainMenuButton->setConnectedElements(nullptr, nullptr, hideGUIButton, resumeButton);

	return true;
}

void PauseGui::shutDown()
{
	delete this->resumeButton;
	delete this->hideGUIButton;
	delete this->mainMenuButton;

	this->selectedElement = nullptr;

	this->resumeButton = nullptr;
	this->hideGUIButton = nullptr;
	this->mainMenuButton = nullptr;

	this->timeSinceChanged = 0.0F;
	this->changedLastFrame = false;
}

bool PauseGui::update(float deltaTime)
{
	bool result = true;
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
				result = false;
			}
			else if (this->selectedElement == this->hideGUIButton)
			{
				this->showGui = !this->showGui;
			}
			else
			{
				result = false;
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
				result = false;
				state->pause(false);
				System::setState(MAINMENU);
			}

			if (System::theTracker->start == DirectX::GamePad::ButtonStateTracker::PRESSED) // start
			{
				state->pause(false);
				result = false;
			}

			if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				if (this->selectedElement == this->resumeButton)
				{
					result = false;
					state->pause(false);
				}
				else if (this->selectedElement == this->hideGUIButton)
				{
					this->showGui = !this->showGui;
				}
				else if(this->selectedElement == this->mainMenuButton)
				{
					result = false;
					state->pause(false);
					System::setState(MAINMENU);
				}
			}

			//break;
		}
	}

	return result;
}

bool PauseGui::render()
{
	if (this->showGui == true)
	{
		System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, System::getSpritebatchMatrix());

		System::getFontArial()->DrawString(System::getSpriteBatch(), "Game Paused", SimpleMath::Vector2(1920 / 2.0F - SimpleMath::Vector2(System::getFontArial()->MeasureString("Game Paused")).x / 2, 1080 / 2.0F - 300), DirectX::Colors::Black, 0.0f, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2::One);
		this->resumeButton->render(this->selectedElement == resumeButton);
		this->hideGUIButton->render(this->selectedElement == hideGUIButton);
		this->mainMenuButton->render(this->selectedElement == mainMenuButton);

		System::getSpriteBatch()->End();
	}
	return this->showGui;
}
