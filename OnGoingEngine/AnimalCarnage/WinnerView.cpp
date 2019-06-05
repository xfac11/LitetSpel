#include "WinnerView.h"
#include "System.h"

Texture WinnerView::texture2 = Texture();
bool WinnerView::texturesLoaded2 = false;

WinnerView::WinnerView(DirectX::SimpleMath::Vector2 position) : LooserView(position)
{
	if (!WinnerView::texturesLoaded2)
	{
		WinnerView::texture2.setTexture("winner.tga");
		WinnerView::texturesLoaded2 = true;
	}
}

WinnerView::~WinnerView()
{
}

bool WinnerView::render(bool selected)
{
	System::getSpriteBatch()->Draw(WinnerView::texture2.getTexture(), this->position);

	switch (this->stats.color)
	{
	case RED:
		System::getSpriteBatch()->Draw(WinnerView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 125), DirectX::XMVECTORF32({ { {1,0.067, 0, 1 } } }));
		break;
	case GOLDEN:
		System::getSpriteBatch()->Draw(WinnerView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 125), DirectX::XMVECTORF32({ { {0.82, 0.788, 0.22, 1 } } }));
		break;
	case BROWN:
		System::getSpriteBatch()->Draw(WinnerView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 125), DirectX::XMVECTORF32({ { {0.392, 0.235, 0, 1 } } }));
		break;
	case GREY:
		System::getSpriteBatch()->Draw(WinnerView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 125), DirectX::XMVECTORF32({ { {0.459, 0.459, 0.459, 1 } } }));
		break;
	case CYAN:
		System::getSpriteBatch()->Draw(WinnerView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 125), DirectX::Colors::Cyan);
		break;
	case PINK:
		System::getSpriteBatch()->Draw(WinnerView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 125), DirectX::XMVECTORF32({ { {1, 0.239, 0.624, 1 } } }));
		break;
	case BLACK:
		System::getSpriteBatch()->Draw(WinnerView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 125), DirectX::XMVECTORF32({ { {0.071, 0.071, 0.071, 1 } } }));
		break;
	case WHITE:
		System::getSpriteBatch()->Draw(WinnerView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 125));
		break;
	case PURPLE:
		System::getSpriteBatch()->Draw(LooserView::backGroundLayer.getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 125), Colors::Purple);
		break;
	}

	switch (this->stats.type)
	{
	case FOX:
		System::getSpriteBatch()->Draw(WinnerView::players[0].getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 125), nullptr);
		break;
	case BEAR:
		System::getSpriteBatch()->Draw(WinnerView::players[1].getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 125), nullptr);
		break;
	case RABBIT:
		System::getSpriteBatch()->Draw(WinnerView::players[2].getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 125), nullptr);
		break;
	case MOOSE:
		System::getSpriteBatch()->Draw(WinnerView::players[3].getTexture(), this->position + DirectX::SimpleMath::Vector2(25, 125), nullptr);
		break;
	}

	System::getFontArial()->DrawString(System::getSpriteBatch(), "Winner", this->position + SimpleMath::Vector2(175.0F, 80.0F), Colors::Black, 0, SimpleMath::Vector2(System::getFontArial()->MeasureString("Winner") / 2.0F), 1);
	System::getFontArial()->DrawString(System::getSpriteBatch(), ("Kills: " + std::to_string(stats.kills)).c_str(), this->position + SimpleMath::Vector2(30.0F, 340.0F + 105.0F), Colors::Black, 0, SimpleMath::Vector2::Zero, SimpleMath::Vector2::One * 0.5F);
	System::getFontArial()->DrawString(System::getSpriteBatch(), ("Deaths: " + std::to_string(stats.deaths)).c_str(), this->position + SimpleMath::Vector2(30.0F, 340.0F + 105.0F + 40.0F), Colors::Black, 0, SimpleMath::Vector2::Zero, SimpleMath::Vector2::One * 0.5F);
	System::getFontArial()->DrawString(System::getSpriteBatch(), "Damage", this->position + SimpleMath::Vector2(350 / 2.0F, 340.0F + 105.0F + 120.0F), Colors::Black, 0, System::getFontArial()->MeasureString("Damage") / 2.0F, SimpleMath::Vector2::One * 0.5F);
	System::getFontArial()->DrawString(System::getSpriteBatch(), ("Taken: " + std::to_string(stats.damageTaken) + "  Dealt: " + std::to_string(stats.damageDealt)).c_str(), this->position + SimpleMath::Vector2(30.0F, 340.0F + 105.0F + 150.0F), Colors::Black, 0, SimpleMath::Vector2::Zero, SimpleMath::Vector2::One * 0.28F);

	return true;
}
