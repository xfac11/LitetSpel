#include "ResultGui.h"
#include "System.h"
#include "GunGameState.h"

ResultGui::ResultGui(State * myState) : GuiBase(myState)
{
	this->buttonMenu = nullptr;
	this->nrOfPlayers = 4;
	this->statsView = new LooserView*[4];
	this->buttonDelay = 0.0f;
	for (int i = 0; i < 4; i++)
	{
		this->statsView[i]=nullptr;
	}
	this->buttonMenu = new Button("Exit to menu", Vector2(1920 / 2.0f - 300, 1080 - 125));
}

ResultGui::~ResultGui()
{
	delete this->buttonMenu;
	for (int i = 0; i < 4; i++)
	{
		if(this->statsView!=nullptr)
			delete this->statsView[i];
	}
	delete[] this->statsView;

	
}

bool ResultGui::initialize()
{
	GunGameState* state = static_cast<GunGameState*>(this->myState);
	for (int i = 0; i < 4; i++)
	{
		if (this->statsView[i] != nullptr)
		{
			delete this->statsView[i];
			this->statsView[i] = nullptr;
		}
	}
	this->nrOfPlayers = state->getNrOfPlayers();
	for (int i = 0; i < this->nrOfPlayers; i++)
	{
		if(i==0)
			this->statsView[0] = new WinnerView(Vector2(1920 / 2.0F - 775, 1080 / 2.0F - 405));
		else if(i==1)
			this->statsView[1] = new LooserView(Vector2(1920 / 2.0F - 375, 1080 / 2.0F - 300));
		else if(i==2)
			this->statsView[2] = new LooserView(Vector2(1920 / 2.0F + 25, 1080 / 2.0F - 300));
		else if(i==3)
			this->statsView[3] = new LooserView(Vector2(1920 / 2.0F + 425, 1080 / 2.0F - 300));
	}

	return true;
}

void ResultGui::shutDown()
{
	delete this->buttonMenu;
	delete this->statsView[0];
	delete this->statsView[1];
	delete this->statsView[2];
	delete this->statsView[3];

	this->buttonMenu = nullptr;
	this->statsView[0] = nullptr;
	this->statsView[1] = nullptr;
	this->statsView[2] = nullptr;
	this->statsView[3] = nullptr;

	this->buttonDelay = 0.0f;
}

bool ResultGui::update(float deltaTime)
{
	if (this->keyboardDelay <= 0.0F)
	{
		if (System::theKeyboard->KeyIsPressed('E'))
		{
			System::setState(MAINMENU);
		}
	}
	else
	{
		this->keyboardDelay -= deltaTime;
	}

	if (this->buttonDelay < 2.0F)
	{
		this->buttonDelay += deltaTime;
	}

	for (int i = 0; i < 4; i++)
	{
		DirectX::GamePad::State gamepadState = System::theGamePad->GetState(i);

		if (gamepadState.IsConnected())
		{
			System::theTracker->Update(gamepadState);

			if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED && this->buttonDelay >= 2.0F)
			{
				System::setState(MAINMENU);
			}
		}
	}

	return true;
}

bool ResultGui::render()
{
	System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, System::getSpritebatchMatrix());

	Vector2 textWidth = System::getFontArial()->MeasureString("Results");
	System::getFontArial()->DrawString(System::getSpriteBatch(), "Results", Vector2(1920 / 2.0F, 100), DirectX::Colors::Black, 0.0f, textWidth / 2.f, Vector2::One);
	this->buttonMenu->render(true);

	for (int i = 0; i < this->nrOfPlayers; i++)
	{
		this->statsView[i]->render(false);
	}

	System::getSpriteBatch()->End();

	return true;
}

void ResultGui::initializePlayerStats()
{
	GunGameState* gunState = static_cast<GunGameState*>(this->myState);
	this->nrOfPlayers = gunState->getNrOfPlayers();
	PlayerStats stats[4];

	for (int i = 0; i < this->nrOfPlayers; i++)
	{
		Player* player = gunState->getPlayer(i);
		stats[i] = player->stats;
		stats[i].playerID = i;
		stats[i].type = player->getAnimalType();
		stats[i].color = player->getColor();
	}

	//Simple bubble sort, speed not important.
	for (int j = 0; j < this->nrOfPlayers - 1; j++)
	{
		for (int i = j; i < this->nrOfPlayers - 1; i++)
		{
			if (stats[i].kills < stats[i + 1].kills)
			{
				PlayerStats copy = stats[i];
				stats[i] = stats[i + 1];
				stats[i + 1] = copy;
			}
		}
	}

	for (int i = 0; i < this->nrOfPlayers; i++)
	{
		this->statsView[i]->setPosition(Vector2(1920 / 2.0F - ((this->nrOfPlayers * 350) / 2.0F) - (((this->nrOfPlayers - 1) * 50) / 2.0F) + i * 400.0F, 1080 / 2.0F - (i == 0 ? 405 : 300)));
		this->statsView[i]->setStats(stats[i]);
	}
}
