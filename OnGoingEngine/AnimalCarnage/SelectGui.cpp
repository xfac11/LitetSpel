#include "SelectGui.h"
#include "System.h"
#include "MainMenu.h"
#include "Player.h"

SelectGui::SelectGui(State * myState) : GuiBase(myState)
{
	this->selectedElement = nullptr;
	this->buttonStart = nullptr;
	this->playerSelectors[0] = nullptr;
	this->playerSelectors[1] = nullptr;
	this->playerSelectors[2] = nullptr;
	this->playerSelectors[3] = nullptr;
}

SelectGui::~SelectGui()
{
	delete this->playerSelectors[0];
	delete this->playerSelectors[1];
	delete this->playerSelectors[2];
	delete this->playerSelectors[3];
	delete this->buttonStart;
}

bool SelectGui::initialize()
{
	this->buttonStart = new Button("Start Game", Vector2(System::theWindow.width / 2.0F - 300, System::theWindow.height / 2.0F + 300));
	this->playerSelectors[0] = new PlayerSelector(FOX, RED, Vector2(System::theWindow.width / 2.0F - 775, System::theWindow.height / 2.0F - 300));
	this->playerSelectors[1] = new PlayerSelector(FOX, BLUE, Vector2(System::theWindow.width / 2.0F - 375, System::theWindow.height / 2.0F - 300));
	this->playerSelectors[2] = new PlayerSelector(FOX, GREEN, Vector2(System::theWindow.width / 2.0F + 25, System::theWindow.height / 2.0F - 300));
	this->playerSelectors[3] = new PlayerSelector(FOX, YELLOW, Vector2(System::theWindow.width / 2.0F + 425, System::theWindow.height / 2.0F - 300));

	this->selectedElement = buttonStart;
	this->buttonStart->setConnectedElements(nullptr, nullptr, nullptr, nullptr);
	return true;
}

void SelectGui::shutDown()
{
	delete this->playerSelectors[0];
	delete this->playerSelectors[1];
	delete this->playerSelectors[2];
	delete this->playerSelectors[3];
	delete this->buttonStart;

	this->selectedElement = nullptr;
	this->buttonStart = nullptr;
	this->playerSelectors[0] = nullptr;
	this->playerSelectors[1] = nullptr;
	this->playerSelectors[2] = nullptr;
	this->playerSelectors[3] = nullptr;
}

bool SelectGui::update(float deltaTime)
{
	if (this->keyboardDelay <= 0.0F)
	{
		if (System::theKeyboard->KeyIsPressed('E'))
		{
			System::setState(GUNGAME);

			AnimalType type[4];
			PlayerColor color[4];

			for (int i = 0; i < 4; i++)
			{
				type[i] = this->playerSelectors[i]->getAnimalType();
				color[i] = this->playerSelectors[i]->getPlayerColor();
			}

			dynamic_cast<GunGameState*>(System::getCurrentState())->initPlayers(type, color);
		}
		else if (System::theKeyboard->KeyIsPressed('Q'))
		{
			MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
			state->setCurrentMenu(MAIN/*RULES*/);
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
				System::setState(GUNGAME);

				AnimalType type[4];
				PlayerColor color[4];

				for (int i = 0; i < 4; i++)
				{
					type[i] = this->playerSelectors[i]->getAnimalType();
					color[i] = this->playerSelectors[i]->getPlayerColor();
				}

				dynamic_cast<GunGameState*>(System::getCurrentState())->initPlayers(type, color);
			}
			else if (System::theTracker->b == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
				state->setCurrentMenu(MAIN/*RULES*/);
			}
			else if (System::theTracker->dpadDown == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				this->playerSelectors[i]->changePlayerColor(false);
			}
			else if (System::theTracker->dpadUp == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				this->playerSelectors[i]->changePlayerColor(true);
			}
			else if (System::theTracker->dpadLeft == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				this->playerSelectors[i]->changeAnimalType(true);
			}
			else if (System::theTracker->dpadRight == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				this->playerSelectors[i]->changeAnimalType(false);
			}
		}
	}

	return true;
}

bool SelectGui::render()
{
	System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, System::getSpritebatchMatrix());

	Vector2 textWidth = System::getFontArial()->MeasureString("Player Select");
	System::getFontArial()->DrawString(System::getSpriteBatch(), "Player Select", Vector2(System::theWindow.width / 2.0F, System::theWindow.height / 2.0F - 400), DirectX::Colors::Black, 0.0f, textWidth / 2.f, Vector2::One);
	this->buttonStart->render(this->selectedElement == buttonStart);
	this->playerSelectors[0]->render(false);
	this->playerSelectors[1]->render(false);
	this->playerSelectors[2]->render(false);
	this->playerSelectors[3]->render(false);

	System::getSpriteBatch()->End();
	return true;
}