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

				System::theRumble[i].rumble.x = 0.2f;
				System::theRumble[i].rumble.y = 0.2f;
				System::theRumble[i].rumbleTime = 0.2f;
			}

			break;
		}
	}

	return true;
}

bool OptionsGui::render()
{
	System::getSpriteBatch()->Begin();
	System::getFontComicSans()->DrawString(System::getSpriteBatch(), "Options", DirectX::SimpleMath::Vector2(200, 200), DirectX::Colors::Black, 0.0f, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2::One * 3);
	System::getSpriteBatch()->Draw(this->volume->getTextureBG(), this->volume->getPosition(), nullptr, this->volume == this->selectedElement ? DirectX::Colors::Red : DirectX::Colors::White);
	System::getSpriteBatch()->Draw(this->volume->getTexture(), DirectX::SimpleMath::Rectangle(static_cast<long>(this->volume->getPosition().x), static_cast<long>(this->volume->getPosition().y), 100 * this->volume->getValue(), 100), nullptr, DirectX::Colors::White);
	System::getSpriteBatch()->End();

	return true;
}
