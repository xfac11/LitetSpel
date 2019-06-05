#include "LooserView.h"
#include "System.h"

Texture LooserView::texture = Texture();
Texture LooserView::players[4] = { Texture(), Texture(), Texture(), Texture() };
Texture LooserView::backGroundLayer = Texture();
bool LooserView::texturesLoaded = false;

LooserView::LooserView(DirectX::SimpleMath::Vector2 position) : GuiElement(position)
{
	if (!LooserView::texturesLoaded)
	{
		LooserView::texture.setTexture("selector.tga");
		LooserView::players[0].setTexture("playerFox.tga");
		LooserView::players[1].setTexture("playerBear.tga");
		LooserView::players[2].setTexture("playerRabbit.tga");
		LooserView::players[3].setTexture("playerMoose.tga");
		LooserView::backGroundLayer.setTexture("white.tga");

		LooserView::texturesLoaded = true;
	}
}

LooserView::~LooserView()
{
}

bool LooserView::render(bool selected)
{
	System::getSpriteBatch()->Draw(LooserView::texture.getTexture(), this->position);

	switch (this->stats.color)
	{
	case RED:
		System::getSpriteBatch()->Draw(LooserView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::XMVECTORF32({ { {1,0.067, 0, 1 } } }));
		break;
	case GOLDEN:
		System::getSpriteBatch()->Draw(LooserView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::XMVECTORF32({ { {0.82, 0.788, 0.22, 1 } } }));
		break;
	case BROWN:
		System::getSpriteBatch()->Draw(LooserView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::XMVECTORF32({ { {0.392, 0.235, 0, 1 } } }));
		break;
	case GREY:
		System::getSpriteBatch()->Draw(LooserView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::XMVECTORF32({ { {0.459, 0.459, 0.459, 1 } } }));
		break;
	case CYAN:
		System::getSpriteBatch()->Draw(LooserView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::Colors::Cyan);
		break;
	case PINK:
		System::getSpriteBatch()->Draw(LooserView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::XMVECTORF32({ { {1, 0.239, 0.624, 1 } } }));
		break;
	case BLACK:
		System::getSpriteBatch()->Draw(LooserView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), DirectX::XMVECTORF32({ { {0.071, 0.071, 0.071, 1 } } }));
		break;
	case WHITE:
		System::getSpriteBatch()->Draw(LooserView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20));
		break;
	case PURPLE:
		System::getSpriteBatch()->Draw(LooserView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), Colors::Purple);
		break;
	}

	switch (this->stats.type)
	{
	case FOX:
		System::getSpriteBatch()->Draw(LooserView::players[0].getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), nullptr);
		break;
	case BEAR:
		System::getSpriteBatch()->Draw(LooserView::players[1].getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), nullptr);
		break;
	case RABBIT:
		System::getSpriteBatch()->Draw(LooserView::players[2].getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), nullptr);
		break;
	case MOOSE:
		System::getSpriteBatch()->Draw(LooserView::players[3].getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 20), nullptr);
		break;
	}

	System::getFontArial()->DrawString(System::getSpriteBatch(), ("Kills: " + std::to_string(stats.kills)).c_str(), this->position + SimpleMath::Vector2(30.0F, 340.0F), Colors::Black, 0, SimpleMath::Vector2::Zero, SimpleMath::Vector2::One * 0.5F);
	System::getFontArial()->DrawString(System::getSpriteBatch(), ("Deaths: " + std::to_string(stats.deaths)).c_str(), this->position + SimpleMath::Vector2(30.0F, 340.0F + 40.0F), Colors::Black, 0, SimpleMath::Vector2::Zero, SimpleMath::Vector2::One * 0.5F);
	System::getFontArial()->DrawString(System::getSpriteBatch(), "Damage", this->position + SimpleMath::Vector2(350 / 2.0F, 340.0F + 120.0F), Colors::Black, 0, System::getFontArial()->MeasureString("Damage") / 2.0F, SimpleMath::Vector2::One * 0.5F);
	System::getFontArial()->DrawString(System::getSpriteBatch(), ("Taken: " + std::to_string(stats.damageTaken) + "  Dealt: " + std::to_string(stats.damageDealt)).c_str(), this->position + SimpleMath::Vector2(30.0F, 340.0F + 150.0F), Colors::Black, 0, SimpleMath::Vector2::Zero, SimpleMath::Vector2::One * 0.28F);

	return true;
}

void LooserView::setStats(PlayerStats stats)
{
	this->stats = stats;
}
