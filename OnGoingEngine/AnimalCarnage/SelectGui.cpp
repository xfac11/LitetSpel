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
		SelectGui::pressStart.setTexture("PressStart.tga");
		SelectGui::texturesLoaded = true;

	}
	this->nrOfPlayers = 0;
	this->playersColor = new PlayerColor[4];
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
	delete[] this->playersColor;
	delete this->buttonStart;
}

bool SelectGui::initialize()
{
	this->buttonStart = new Button("Press start to enter game", Vector2(1920 / 2.0F - 300, 1080 / 2.0F + 300));
	this->playerSelectors[0] = new PlayerSelector(FOX, RED,0, Vector2(1920 / 2.0F - 775, 1080 / 2.0F - 300));
	this->playerSelectors[1] = new PlayerSelector(FOX, GOLDEN,1, Vector2(1920 / 2.0F - 375, 1080 / 2.0F - 300));
	this->playerSelectors[2] = new PlayerSelector(FOX, BROWN,2, Vector2(1920 / 2.0F + 25, 1080 / 2.0F - 300));
	this->playerSelectors[3] = new PlayerSelector(FOX, GREY,3, Vector2(1920 / 2.0F + 425, 1080 / 2.0F - 300));
	for (int i = 0; i < 4; i++)
	{
		this->playersColor[i] = this->playerSelectors[i]->getPlayerColor();
	}
	this->selectedElement = buttonStart;
	System::theTracker->Reset();
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
	int ready = 0;
	if (this->keyboardDelay <= 0.0F)
	{
		if (System::theKeyboard->KeyIsPressed('E'))
		{
			System::setState(GUNGAME);

			AnimalType type[4];
			PlayerColor color[4];
			bool rumble[4];

			for (int i = 0; i < 4; i++)
			{
				type[i] = this->playerSelectors[i]->getAnimalType();
				color[i] = this->playerSelectors[i]->getPlayerColor();
			}

			MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
			state->getRumble(rumble);
			dynamic_cast<GunGameState*>(System::getCurrentState())->initPlayers(this->nrOfPlayers,type, color, rumble);
		}
		else if (System::theKeyboard->KeyIsPressed('Q'))
		{
			MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
			state->setCurrentMenu(MAIN/*RULES*/);
			return true;
		}
		
		
		int nrOfConnected = 0;
		for (int i = 0; i < 4; i++)
		{
			this->playerSelectors[i]->update(this->playersColor, this->nrOfPlayers);
		}
		//for (int i = 0; i < 4; i++)
		//{
		//	DirectX::GamePad::State gamepadState = System::theGamePad->GetState(i);
		//	
		//	if (gamepadState.IsConnected())
		//	{
		//		nrOfConnected++;
		//		System::theTracker->Update(gamepadState);

		//		if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::RELEASED)
		//		{
		//			/*if(this->playerSelectors[0]->getPlayerColor()== this->playerSelectors[]->getPlayerColor())*/
		//				this->playerSelectors[i]->setReady(true);
		//		}
		//		else if (System::theTracker->b == DirectX::GamePad::ButtonStateTracker::RELEASED)
		//		{
		//			if (this->playerSelectors[i]->getReady())
		//			{
		//				this->playerSelectors[i]->setReady(false);
		//			}
		//			else if (!this->playerSelectors[i]->getReady())
		//			{
		//				MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
		//				state->setCurrentMenu(MAIN/*RULES*/);
		//				return true;
		//			}
		//		}
		//		else if (!this->playerSelectors[i]->getReady()&&System::theTracker->dpadDown == DirectX::GamePad::ButtonStateTracker::RELEASED)
		//		{
		//				this->playerSelectors[i]->changePlayerColor(false);
		//				while ((i != 0 && this->playerSelectors[i]->getPlayerColor() == this->playerSelectors[0]->getPlayerColor()) ||
		//					(i != 1 && this->playerSelectors[i]->getPlayerColor() == this->playerSelectors[1]->getPlayerColor()) ||
		//					(i != 2 && this->playerSelectors[i]->getPlayerColor() == this->playerSelectors[2]->getPlayerColor()) ||
		//					(i != 3 && this->playerSelectors[i]->getPlayerColor() == this->playerSelectors[3]->getPlayerColor()))
		//				{
		//					this->playerSelectors[i]->changePlayerColor(false);
		//				}
		//		}
		//		else if (!this->playerSelectors[i]->getReady() &&System::theTracker->dpadUp == DirectX::GamePad::ButtonStateTracker::RELEASED)
		//		{
		//				this->playerSelectors[i]->changePlayerColor(true);
		//				while ((i != 0 && this->playerSelectors[i]->getPlayerColor() == this->playerSelectors[0]->getPlayerColor())||
		//					(i != 1 && this->playerSelectors[i]->getPlayerColor() == this->playerSelectors[1]->getPlayerColor())||
		//					(i != 2 && this->playerSelectors[i]->getPlayerColor() == this->playerSelectors[2]->getPlayerColor())||
		//					(i != 3 && this->playerSelectors[i]->getPlayerColor() == this->playerSelectors[3]->getPlayerColor()))
		//				{
		//					this->playerSelectors[i]->changePlayerColor(true);
		//				}
		//		}
		//		else if (!this->playerSelectors[i]->getReady()&&System::theTracker->dpadLeft == DirectX::GamePad::ButtonStateTracker::PRESSED)
		//		{
		//				this->playerSelectors[i]->changeAnimalType(true);
		//		}
		//		else if (!this->playerSelectors[i]->getReady()&&System::theTracker->dpadRight == DirectX::GamePad::ButtonStateTracker::PRESSED)
		//		{
		//				this->playerSelectors[i]->changeAnimalType(false);
		//		}
		//	
		//	}
		//}
		//
		for (int i = 0; i < this->nrOfPlayers; i++)
		{
			if (this->playerSelectors[i]->getReady())
				ready++;
		}

		for (int i = 0; i < nrOfPlayers; i++)
		{
			DirectX::GamePad::State gamepadState = System::theGamePad->GetState(i);
			if (gamepadState.IsConnected())
			{
				System::theTracker->Update(gamepadState);
				if (this->nrOfPlayers == ready)
				{
					this->allReady = true;
					if (System::theTracker->start)
					{
						System::setState(GUNGAME);

						AnimalType type[4];
						PlayerColor color[4];
						bool rumble[4];

						for (int i = 0; i < dynamic_cast<GunGameState*>(System::getCurrentState())->getNrOfPlayers(); i++)
						{
							type[i] = this->playerSelectors[i]->getAnimalType();
							color[i] = this->playerSelectors[i]->getPlayerColor();
						}
						this->allReady = false;
						MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
						state->getRumble(rumble);
						//dynamic_cast<GunGameState*>(System::getCurrentState())->initailize();
						if (this->nrOfPlayers == 1)
							this->nrOfPlayers++;
						dynamic_cast<GunGameState*>(System::getCurrentState())->initPlayers(this->nrOfPlayers,type, color, rumble);
						return true;
					}

				}
				else
				{
					if (System::theTracker->view)
					{
						MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
						state->setCurrentMenu(MAIN/*RULES*/);
						return true;
					}
				}
			}
		}
	}
	else
	{
		this->keyboardDelay -= deltaTime;
	}
	if (this->nrOfPlayers > ready)
	{
		this->allReady = false;
	}
	
	this->nrOfPlayers = 0;
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
	Vector2 textWidth2 = System::getFontArial()->MeasureString("Press Start to Begin");
	if (this->allReady)
		System::getFontArial()->DrawString(System::getSpriteBatch(), "Press Start to Begin", Vector2(1920 / 2.0F, 1080 / 2.0F - 100), Colors::Red,-3.14/8, textWidth2/2.0f);
		//System::getSpriteBatch()->Draw(SelectGui::pressStart.getTexture(), Vector2(500, 500), nullptr);//texture would be nicer?
	Vector2 textWidth3 = System::getFontArial()->MeasureString("Press Select to go to  Main Menu");
	System::getFontArial()->DrawString(System::getSpriteBatch(), "Press Select to go to Main Menu", Vector2(250, 1080 / 2.0F+400), Colors::BurlyWood, 0, textWidth3 / 2.0f,Vector2(0.25,0.25));
	System::getSpriteBatch()->End();
	return true;
}