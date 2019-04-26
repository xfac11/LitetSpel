#include "OptionsGui.h"
#include "System.h"
#include "MainMenu.h"

OptionsGui::OptionsGui(State * myState) : GuiBase(myState)
{
	this->selectedElement = nullptr;

	this->volume = nullptr;

	this->pressedLastFrame = false;
}

OptionsGui::~OptionsGui()
{
	delete this->volume;
}

bool OptionsGui::initialize()
{
	this->volume = new Slider(0, 10, 1, DirectX::SimpleMath::Vector2(200, 400));

	this->selectedElement = this->volume;
	this->volume->setConnectedElements(nullptr, nullptr, nullptr, nullptr);

	return true;
}

void OptionsGui::shutDown()
{
	delete this->volume;
}

bool OptionsGui::update(float deltaTime)
{
	if (this->keyboardDelay <= 0.0F)
	{
		if (System::theKeyboard->KeyIsPressed('E'))
		{
			if (!this->pressedLastFrame)
			{
				if (this->volume->getValue() < this->volume->getMaxValue())
				{
					this->volume->changeValueWithStep(false);
				}
				else
				{
					this->volume->setValue(0);
				}
			}

			this->pressedLastFrame = true;
		}
		else
		{
			this->pressedLastFrame = false;

			if (System::theKeyboard->KeyIsPressed('Q'))
			{
				MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
				state->setCurrentMenu(MAIN);
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

			if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				if (this->volume->getValue() < this->volume->getMaxValue())
				{
					this->volume->changeValueWithStep(false);
				}
				else
				{
					this->volume->setValue(0);
				}
			}
			else if (System::theTracker->b == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
				state->setCurrentMenu(MAIN);
			}

			break;
		}
	}

	return true;
}

bool OptionsGui::render()
{
	System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied());
	
	System::getFontComicSans()->DrawString(System::getSpriteBatch(), "Options", DirectX::SimpleMath::Vector2(200, 200), DirectX::Colors::Black, 0.0f, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2::One * 3);
	this->volume->render(this->volume == this->selectedElement);
	
	System::getSpriteBatch()->End();

	return true;
}
