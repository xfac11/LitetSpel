#include "SelectGui.h"
#include "System.h"
#include "MainMenu.h"
#include "Player.h"

Texture SelectGui::pressStart = Texture();
bool SelectGui::texturesLoaded = false;
SelectGui::SelectGui(State * myState) : GuiBase(myState)
{

	if (!SelectGui::texturesLoaded)
	{
		SelectGui::pressStart.setTexture("selector.tga");
		SelectGui::texturesLoaded = true;

	}

	this->selectedElement = nullptr;
	this->buttonStart = nullptr;
	this->playerSelectors[0] = nullptr;
	this->playerSelectors[1] = nullptr;
	this->playerSelectors[2] = nullptr;
	this->playerSelectors[3] = nullptr;
	this->allReady = false;
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
	this->buttonStart = new Button("Press start to enter game", Vector2(1920 / 2.0F - 300, 1080 / 2.0F + 300));
	this->playerSelectors[0] = new PlayerSelector(FOX, RED, Vector2(1920 / 2.0F - 775, 1080 / 2.0F - 300));
	this->playerSelectors[1] = new PlayerSelector(FOX, BLUE, Vector2(1920 / 2.0F - 375, 1080 / 2.0F - 300));
	this->playerSelectors[2] = new PlayerSelector(FOX, GREEN, Vector2(1920 / 2.0F + 25, 1080 / 2.0F - 300));
	this->playerSelectors[3] = new PlayerSelector(FOX, YELLOW, Vector2(1920 / 2.0F + 425, 1080 / 2.0F - 300));

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

	//DirectX::GamePad::State gamepadState = System::theGamePad->GetState(0);
	//if (gamepadState.IsConnected())
	//{
	//	System::theTracker->Update(gamepadState);

	//	if (this->playerSelectors[0]->getReady() && System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED)
	//	{
	//		System::setState(GUNGAME);

	//		AnimalType type[4];
	//		PlayerColor color[4];

	//		for (int i = 0; i < dynamic_cast<GunGameState*>(System::getCurrentState())->getNrOfPlayers(); i++)
	//		{
	//			type[i] = this->playerSelectors[i]->getAnimalType();
	//			color[i] = this->playerSelectors[i]->getPlayerColor();
	//		}

	//		dynamic_cast<GunGameState*>(System::getCurrentState())->initPlayers(type, color);
	//	}
	//}


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
	int nrOfConnected = 0;
	for (int i = 0; i < 4; i++)
	{
		DirectX::GamePad::State gamepadState = System::theGamePad->GetState(i);
		if (gamepadState.IsConnected())
		{
			nrOfConnected++;
			System::theTracker->Update(gamepadState);

			if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				this->playerSelectors[i]->setReady(true);
			}
			else if (System::theTracker->b == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				if (this->playerSelectors[i]->getReady())
				{
					this->playerSelectors[i]->setReady(false);
				}
				else if (!this->playerSelectors[i]->getReady())
				{
					MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
					state->setCurrentMenu(MAIN/*RULES*/);
					return true;
				}
			}
			if (!this->playerSelectors[i]->getReady())
			{
				if (System::theTracker->dpadDown == DirectX::GamePad::ButtonStateTracker::PRESSED)
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
	}
	
	int ready = 0;
	for (int i = 0; i < nrOfConnected; i++)
	{
		if (this->playerSelectors[i]->getReady())
			ready++;
	}
	if (nrOfConnected == ready)
	{
		this->allReady = true;
		for (int i = 0; i < ready; i++)
		{
			DirectX::GamePad::State gamepadState = System::theGamePad->GetState(i);

			if (gamepadState.IsConnected())
			{
				System::theTracker->Update(gamepadState);
				if (System::theTracker->start)
				{
					System::setState(GUNGAME);

					AnimalType type[4];
					PlayerColor color[4];

						for (int i = 0; i < dynamic_cast<GunGameState*>(System::getCurrentState())->getNrOfPlayers(); i++)
						{
							type[i] = this->playerSelectors[i]->getAnimalType();
							color[i] = this->playerSelectors[i]->getPlayerColor();
						}

						dynamic_cast<GunGameState*>(System::getCurrentState())->initPlayers(type, color);
				}
			}
		}
	}
	else
	{
		this->allReady = false;
	}
	

	return true;
}

bool SelectGui::render()
{
	System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, System::getSpritebatchMatrix());

	Vector2 textWidth = System::getFontArial()->MeasureString("Player Select");
	System::getFontArial()->DrawString(System::getSpriteBatch(), "Player Select", Vector2(1920 / 2.0F, 1080 / 2.0F - 400), DirectX::Colors::Black, 0.0f, textWidth / 2.f, Vector2::One);
	
		//this->buttonStart->render(this->selectedElement == buttonStart);
	this->playerSelectors[0]->render(false);
	this->playerSelectors[1]->render(false);
	this->playerSelectors[2]->render(false);
	this->playerSelectors[3]->render(false);

	if (this->allReady)
		System::getSpriteBatch()->Draw(SelectGui::pressStart.getTexture(), Vector2(300, 300), nullptr);

	System::getSpriteBatch()->End();
	return true;
}