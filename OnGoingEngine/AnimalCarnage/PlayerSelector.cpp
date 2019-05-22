#include "PlayerSelector.h"
#include "System.h"

bool PlayerSelector::texturesLoaded = false;
Texture PlayerSelector::selectorBG = Texture();
Texture PlayerSelector::playerCircle = Texture();
Texture PlayerSelector::arrowLeft = Texture();
Texture PlayerSelector::arrowRight = Texture();
Texture PlayerSelector::playerFox = Texture();
//Texture PlayerSelector::backGroundPLayer = Texture();

PlayerSelector::PlayerSelector(Players currentPlayer, DirectX::SimpleMath::Vector2 position) : GuiElement(position), currentPlayer(currentPlayer)
{
	if (!PlayerSelector::texturesLoaded)
	{
		PlayerSelector::selectorBG.setTexture("selector.tga");
		PlayerSelector::playerCircle.setTexture("playerCircle.tga");
		PlayerSelector::arrowLeft.setTexture("arrowLeft.tga");
		PlayerSelector::arrowRight.setTexture("arrowRight.tga");
		PlayerSelector::playerFox.setTexture("foxTest.tga");
		//PlayerSelector::backGroundPLayer.setTexture("foxTest.tga");//background
		
		PlayerSelector::texturesLoaded = true;
	}
	this->backGroundColor = DirectX::XMVectorSet(0, 1, 0, 1);
}

PlayerSelector::~PlayerSelector()
{
}

bool PlayerSelector::render(bool selected)
{
	//DirectX::XMVECTOR vc=DirectX::XMVectorSet()
	System::getSpriteBatch()->Draw(PlayerSelector::selectorBG.getTexture(), this->position, nullptr);
	System::getSpriteBatch()->Draw(PlayerSelector::playerFox.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20),this->backGroundColor);
	System::getSpriteBatch()->Draw(PlayerSelector::playerCircle.getTexture(), this->position + DirectX::SimpleMath::Vector2(10, 10), nullptr);
	System::getSpriteBatch()->Draw(PlayerSelector::arrowLeft.getTexture(), this->position + DirectX::SimpleMath::Vector2(30, 145), nullptr);
	System::getSpriteBatch()->Draw(PlayerSelector::arrowRight.getTexture(), this->position + DirectX::SimpleMath::Vector2(270, 145), nullptr);

	return true;
}
