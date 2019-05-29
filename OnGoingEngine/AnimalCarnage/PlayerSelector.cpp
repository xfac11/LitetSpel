#include "PlayerSelector.h"
#include "System.h"

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

PlayerSelector::PlayerSelector(AnimalType animalType, PlayerColor color, DirectX::SimpleMath::Vector2 position) : GuiElement(position), animalType(animalType), color(color)
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
	this->readyColor = Colors::Red;
	this->ready = false;
}

PlayerSelector::~PlayerSelector()
{
}

bool PlayerSelector::render(bool selected,DirectX::XMVECTOR color)
{
	System::getSpriteBatch()->Draw(PlayerSelector::selectorBG.getTexture(), this->position, nullptr);

	switch (this->color)
	{
	case RED:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::Colors::Red);
		break;
	case BLUE:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::Colors::Blue);
		break;
	case GREEN:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::Colors::Green);
		break;
	case YELLOW:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::Colors::Yellow);
		break;
	case CYAN:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::Colors::Cyan);
		break;
	case PINK:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::Colors::Pink);
		break;
	case BLACK:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::Colors::Black);
		break;
	case WHITE:
		System::getSpriteBatch()->Draw(PlayerSelector::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20));
		break;
	}

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

void PlayerSelector::setReady(bool arg)
{
	this->ready = arg;
	if (arg)
		this->readyColor = Colors::Green;
	else
		this->readyColor = Colors::Red;
}

void PlayerSelector::changeAnimalType(bool dir)
{
	int animalType = static_cast<int>(this->animalType) + (dir ? -1 : 1);

	if (animalType < 0)
		animalType = 3;
	else if (animalType > 3)
		animalType = 0;

	this->animalType = static_cast<AnimalType>(animalType);
}

#include <iostream>
void PlayerSelector::changePlayerColor(bool dir)
{
	int color = static_cast<int>(this->color) + (dir ? -1 : 1);

	if (color < 0)
		color = 7;
	else if (color > 7)
		color = 0;

	this->color = static_cast<PlayerColor>(color);
}
