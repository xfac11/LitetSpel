#include "PlayerSelector.h"
#include "System.h"

bool PlayerSelector::texturesLoaded = false;
Texture PlayerSelector::selectorBG = Texture();
Texture PlayerSelector::playerCircle = Texture();
Texture PlayerSelector::arrowLeft = Texture();
Texture PlayerSelector::arrowRight = Texture();

PlayerSelector::PlayerSelector(Players currentPlayer, DirectX::SimpleMath::Vector2 position) : GuiElement(position), currentPlayer(currentPlayer)
{
	if (!PlayerSelector::texturesLoaded)
	{
		PlayerSelector::selectorBG.setTexture("selector.tga");
		PlayerSelector::playerCircle.setTexture("playerCircle.tga");
		PlayerSelector::arrowLeft.setTexture("arrowLeft.tga");
		PlayerSelector::arrowRight.setTexture("arrowRight.tga");

		PlayerSelector::texturesLoaded = true;
	}
}

PlayerSelector::~PlayerSelector()
{
}

bool PlayerSelector::render(bool selected)
{
	System::getSpriteBatch()->Draw(PlayerSelector::selectorBG.getTexture(), this->position, nullptr);
	System::getSpriteBatch()->Draw(PlayerSelector::playerCircle.getTexture(), this->position + DirectX::SimpleMath::Vector2(10, 10), nullptr);
	System::getSpriteBatch()->Draw(PlayerSelector::arrowLeft.getTexture(), this->position + DirectX::SimpleMath::Vector2(30, 145), nullptr);
	System::getSpriteBatch()->Draw(PlayerSelector::arrowRight.getTexture(), this->position + DirectX::SimpleMath::Vector2(270, 145), nullptr);

	return true;
}
