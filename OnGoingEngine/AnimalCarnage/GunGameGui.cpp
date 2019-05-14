#include "GunGameGui.h"
#include "System.h"
#include "GunGameState.h"

GunGameGui::GunGameGui(State* myState) : GuiBase(myState)
{
	this->nrOfPlayers = 0;
	this->playerHealthBars = nullptr;
	
}

GunGameGui::~GunGameGui()
{
	for (int i = 0; i < this->nrOfPlayers; i++)
	{
		delete this->playerHealthBars[i];
	}

	delete[] this->playerHealthBars;
}

bool GunGameGui::initialize()
{
	GunGameState* state = static_cast<GunGameState*>(this->myState);

	this->nrOfPlayers = state->getNrOfPlayers();
	this->playerHealthBars = new HealthBar*[this->nrOfPlayers];

	for (int i = 0; i < this->nrOfPlayers; i++)
	{
		this->playerHealthBars[i] = new HealthBar(/*state->getPlayer(id)->getHealth(), state->getPlayer(id)->getMaxHealth()*/100, 100, 
			SimpleMath::Vector2(WIDTH / 2 - ((this->nrOfPlayers * 300) / 2) - (((this->nrOfPlayers - 1) * 50) / 2) + i * 350.0F, HEIGHT - 150.0F));
	}

	return true;
}

void GunGameGui::shutDown()
{
	for (int i = 0; i < this->nrOfPlayers; i++)
	{
		delete this->playerHealthBars[i];
	}

	delete[] this->playerHealthBars;

	this->nrOfPlayers = 0;
	this->playerHealthBars = nullptr;
}

bool GunGameGui::update(float deltaTime)
{
	GunGameState* state = static_cast<GunGameState*>(this->myState);

	for (int i = 0; i < this->nrOfPlayers; i++)
	{
		//this->playerHealthBars[i]->setHealth(state->getPlayer(i)->getHealth());
		//this->playerHealthBars[i]->setMaxHealth(state->getPlayer(i)->getMaxHealth());
	}

	if (System::theKeyboard->KeyIsPressed('Q'))
	{
		state->pause(true);
	}
	if (System::theKeyboard->KeyIsPressed('P'))
	{
		state->pause(false);
	}

	for (int i = 0; i < 4; i++)
	{
		DirectX::GamePad::State gamepadState = System::theGamePad->GetState(i);

		if (gamepadState.IsConnected())
		{
			//System::theTracker->Update(gamepadState);

			if (System::theTracker->start == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				state->pause(true);
			}
			break;
		}
	}

	return true;
}

bool GunGameGui::render()
{
	System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied());

	for (int i = 0; i < this->nrOfPlayers; i++)
	{
		System::getFontArial()->DrawString(System::getSpriteBatch(), ("Player " + std::to_string(i + 1)).c_str(), SimpleMath::Vector2(WIDTH / 2 - ((this->nrOfPlayers * 300) / 2) - (((this->nrOfPlayers - 1) * 50) / 2) + i * 350.0F, HEIGHT - 210.0F), Colors::Black, 0.0f, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2::One / 2);
		this->playerHealthBars[i]->render(false);
	}

	System::getSpriteBatch()->End();
	return true;
}