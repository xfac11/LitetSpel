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
		this->playerHealthBars[i] = new HealthBar(state->getPlayer(i)->getHealth(), state->getPlayer(i)->getMaxHealth(), 
			SimpleMath::Vector2(1920 / 2 - ((this->nrOfPlayers * 300) / 2) - (((this->nrOfPlayers - 1) * 50) / 2) + i * 350.0F, 10.0F));
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
	//Reminder : insert next to playerhealth so it displays the right paws/crown
	for (int i = 0; i < this->nrOfPlayers; i++)
	{
		this->playerHealthBars[i]->setHealth(state->getPlayer(i)->getHealth());
		this->playerHealthBars[i]->setMaxHealth(state->getPlayer(i)->getMaxHealth());
		this->playerHealthBars[i]->setColor(state->getPlayer(i)->playerObj->getColorMask().x, state->getPlayer(i)->playerObj->getColorMask().y, state->getPlayer(i)->playerObj->getColorMask().z);
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
	System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, System::getSpritebatchMatrix());
	
	GunGameState* state = static_cast<GunGameState*>(this->myState);
	for (int i = 0; i < this->nrOfPlayers; i++)
	{
		System::getFontArial()->DrawString(System::getSpriteBatch(), ("Next: " + std::to_string(state->getPlayer(i)->getNextAnimal())).c_str(), SimpleMath::Vector2(1920 / 2.0F - ((this->nrOfPlayers * 300) / 2) - (((this->nrOfPlayers - 1) * 50) / 2) + i * 350.0F, 70.0F + 35), Colors::Black, 0.0f, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2::One / 2);
		
		const char* output = ("P " + std::to_string(i + 1)).c_str();
		SimpleMath::Vector2 origin = System::getFontArial()->MeasureString("P2") / 2.f;

		SimpleMath::Vector2 pos = SimpleMath::Vector2(1920 / 2.0F - ((this->nrOfPlayers * 300) / 2) - (((this->nrOfPlayers - 1) * 50) / 2) + i * 350.0F, 150.0F);

		//SimpleMath::Vector2 pos = SimpleMath::Vector2(300, 300);
		this->playerHealthBars[i]->render(false);
		/*System::getFontArial()->DrawString(System::getSpriteBatch(), ("P" + std::to_string(i + 1)).c_str(),
			pos + SimpleMath::Vector2(1.f, 1.f), Colors::GhostWhite, 0.f, origin, DirectX::SimpleMath::Vector2::One / 3);
		System::getFontArial()->DrawString(System::getSpriteBatch(), ("P" + std::to_string(i + 1)).c_str(),
			pos + SimpleMath::Vector2(-1.f, 1.f), Colors::GhostWhite, 0.f, origin, DirectX::SimpleMath::Vector2::One / 3);
		System::getFontArial()->DrawString(System::getSpriteBatch(), ("P" + std::to_string(i + 1)).c_str(),
			pos + SimpleMath::Vector2(-1.f, -1.f), Colors::GhostWhite, 0.f, origin, DirectX::SimpleMath::Vector2::One / 3);*/
		System::getFontArial()->DrawString(System::getSpriteBatch(), ("P" + std::to_string(i + 1)).c_str(),
			pos, Colors::GhostWhite, 0.f, origin, (DirectX::SimpleMath::Vector2::One / 3)*1.1);

		System::getFontArial()->DrawString(System::getSpriteBatch(), ("P" + std::to_string(i + 1)).c_str(), pos, this->playerHealthBars[i]->getColor() /*Colors::ForestGreen*/, 0.0f, origin, DirectX::SimpleMath::Vector2::One / 3);
	}

	System::getSpriteBatch()->End();
	return true;
}
