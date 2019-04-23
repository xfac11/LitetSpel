#include "OptionsGui.h"
#include "System.h"
#include "MainMenu.h"

OptionsGui::OptionsGui(State * myState) : GuiBase(myState)
{
	this->selectedElement = nullptr;
}

OptionsGui::~OptionsGui()
{
}

bool OptionsGui::initialize()
{
	return true;
}

void OptionsGui::shutDown()
{
}

bool OptionsGui::update(float deltaTime)
{
	for (int i = 0; i < 4; i++)
	{
		DirectX::GamePad::State gamepadState = System::theGamePad->GetState(i);

		if (gamepadState.IsConnected())
		{
			System::theTracker->Update(gamepadState);

			if (System::theTracker->b == DirectX::GamePad::ButtonStateTracker::PRESSED)
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
	System::getSpriteBatch()->End();

	return true;
}
