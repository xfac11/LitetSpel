#include "PlayerSelector.h"
#include "System.h"
#include "MainMenu.h"
#include "Player.h"
bool PlayerSelector::texturesLoaded = false;
Texture PlayerSelector::selectorBG = Texture();
Texture PlayerSelector::playerCircle = Texture();
Texture PlayerSelector::arrowLeft = Texture();
Texture PlayerSelector::arrowRight = Texture();
Texture PlayerSelector::backGroundLayer = Texture();
Texture PlayerSelector::players[4] = { Texture(), Texture(), Texture(), Texture() };

//FOX,
//BEAR,
//RABBIT,
//MOOSE

PlayerSelector::PlayerSelector(AnimalType animalType, PlayerColor color,int id, DirectX::SimpleMath::Vector2 position) : GuiElement(position), animalType(animalType), color(color)
{
	if (!PlayerSelector::texturesLoaded)
	{
		PlayerSelector::selectorBG.setTexture("selector.tga");
		PlayerSelector::playerCircle.setTexture("playerCircle.tga");
		PlayerSelector::arrowLeft.setTexture("arrowLeft.tga");
		PlayerSelector::arrowRight.setTexture("arrowRight.tga");
		PlayerSelector::players[0].setTexture("playerFox.tga");
		PlayerSelector::players[1].setTexture("playerBear.tga");
		PlayerSelector::players[2].setTexture("playerRabbit.tga");
		PlayerSelector::players[3].setTexture("playerMoose.tga");
		PlayerSelector::backGroundLayer.setTexture("white.tga");
		
		PlayerSelector::texturesLoaded = true;
	}

	this->controllerID = id;
	this->readyColor = Colors::DarkGray;
	this->ready = false;
	this->connected = false;
}

PlayerSelector::~PlayerSelector()
{
	
}

bool PlayerSelector::render(bool selected, DirectX::XMVECTOR color)
{
	if(this->connected)
		System::getSpriteBatch()->Draw(PlayerSelector::selectorBG.getTexture(), this->position, nullptr);
	else
	{
		System::getSpriteBatch()->Draw(PlayerSelector::selectorBG.getTexture(), this->position, Colors::Gray);
	}
	switch (this->color)
	{
	case RED:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::XMVECTORF32({ { {1,0.067, 0, 1 } } }));
		break;
	case GOLDEN:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::XMVECTORF32({ { {0.82, 0.788, 0.22, 1 } } }));
		break;
	case BROWN:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::XMVECTORF32({ { {0.392, 0.235, 0, 1 } } }));
		break;
	case GREY:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::XMVECTORF32({ { {0.459, 0.459, 0.459, 1 } } }));
		break;
	case CYAN:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::Colors::Cyan);
		break;
	case PINK:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::XMVECTORF32({ { {1, 0.239, 0.624, 1 } } }));
		break;
	case BLACK:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::XMVECTORF32({ { {0.071, 0.071, 0.071, 1 } } }));
		break;
	case WHITE:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20));
		break;
	case PURPLE:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20),Colors::Purple);
		break;
	}
	//System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20),color);
	switch (this->animalType)
	{
	case FOX:
		System::getSpriteBatch()->Draw(PlayerSelector::players[0].getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), nullptr);
		break;
	case BEAR:
		System::getSpriteBatch()->Draw(PlayerSelector::players[1].getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), nullptr);
		break;
	case RABBIT:
		System::getSpriteBatch()->Draw(PlayerSelector::players[2].getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), nullptr);
		break;
	case MOOSE:
		System::getSpriteBatch()->Draw(PlayerSelector::players[3].getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), nullptr);
		break;
	}
	
	System::getSpriteBatch()->Draw(PlayerSelector::playerCircle.getTexture(), this->position + DirectX::SimpleMath::Vector2(10, 10), readyColor);
	System::getSpriteBatch()->Draw(PlayerSelector::arrowLeft.getTexture(), this->position + DirectX::SimpleMath::Vector2(30, 145), nullptr);
	System::getSpriteBatch()->Draw(PlayerSelector::arrowRight.getTexture(), this->position + DirectX::SimpleMath::Vector2(270, 145), nullptr);

	return true;
}

bool PlayerSelector::getReady()
{
	return this->ready;
}

AnimalType PlayerSelector::getAnimalType() const
{
	return this->animalType;
}

PlayerColor PlayerSelector::getPlayerColor() const
{
	return this->color;
}

void PlayerSelector::update(PlayerColor *& arr, int& nrOfPlayers)
{
	DirectX::GamePad::State gamepadState = System::theGamePad->GetState(this->controllerID);
	int i = 0;
	if (gamepadState.IsConnected())
	{
		this->connected = true;
		mButtons.Update(gamepadState);
		using ButtonState = GamePad::ButtonStateTracker::ButtonState;
		arr[controllerID] = getPlayerColor();
		nrOfPlayers++;
		System::theTracker->Update(gamepadState);
		if (mButtons.a == ButtonState::PRESSED)// == DirectX::GamePad::ButtonStateTracker::RELEASED)
		{
			/*if(this->playerSelectors[0]->getPlayerColor()== this->playerSelectors[]->getPlayerColor())*/
			setReady(true);
		}
		else if (mButtons.b==ButtonState::PRESSED)//System::theTracker->b == DirectX::GamePad::ButtonStateTracker::RELEASED)
		{
			//move to selectgui? or return something that tells this has pressed b and is not ready
			if (getReady())
			{
				setReady(false);
			}
		}
		else if (!getReady() && mButtons.dpadDown==ButtonState::PRESSED)//(gamepadState.IsDPadDownPressed()&&(System::theTracker->dpadDown == DirectX::GamePad::ButtonStateTracker::RELEASED)))// == DirectX::GamePad::ButtonStateTracker::RELEASED)
		{
			changePlayerColor(false);
			while ((this->controllerID != 0 && getPlayerColor() == arr[0]) ||
				(this->controllerID != 1 && getPlayerColor() == arr[1]) ||
				(this->controllerID != 2 && getPlayerColor() == arr[2]) ||
				(this->controllerID != 3 && getPlayerColor() == arr[3]))
			{
				changePlayerColor(false);
			}
		}
		else if (!getReady() && mButtons.dpadUp == ButtonState::PRESSED)//System::theTracker->dpadUp == DirectX::GamePad::ButtonStateTracker::RELEASED)
		{
			changePlayerColor(true);
			while ((this->controllerID != 0 && getPlayerColor() == arr[0]) ||
				(this->controllerID != 1 && getPlayerColor() == arr[1]) ||
				(this->controllerID != 2 && getPlayerColor() == arr[2]) ||
				(this->controllerID != 3 && getPlayerColor() == arr[3]))
			{
				changePlayerColor(true);
			}
		}
		else if (!getReady() && mButtons.dpadLeft == ButtonState::PRESSED)//System::theTracker->dpadLeft == DirectX::GamePad::ButtonStateTracker::PRESSED)
		{
			changeAnimalType(true);
		}
		else if (!getReady() && mButtons.dpadRight == ButtonState::PRESSED)//System::theTracker->dpadRight == DirectX::GamePad::ButtonStateTracker::PRESSED)
		{
			changeAnimalType(false);
		}
	}
	else
	{
		this->connected = false;
	}
	arr[controllerID] = getPlayerColor();
}

void PlayerSelector::setReady(bool arg)
{
	this->ready = arg;
	if (arg)
		this->readyColor = Colors::Lime;
	else
		this->readyColor = Colors::DarkGray;
}

void PlayerSelector::changeAnimalType(bool dir)
{
	int animalType = static_cast<int>(this->animalType) + (dir ? -1 : 1);

	if (animalType < 0)
		animalType = 3;
	else if (animalType > 3)
		animalType = 0;//s

	this->animalType = static_cast<AnimalType>(animalType);
}

#include <iostream>
void PlayerSelector::changePlayerColor(bool dir)
{
	int color = static_cast<int>(this->color) + (dir ? -1 : 1);

	if (color < 0)
		color = 8;
	else if (color > 8)
		color = 0;

	this->color = static_cast<PlayerColor>(color);
}
