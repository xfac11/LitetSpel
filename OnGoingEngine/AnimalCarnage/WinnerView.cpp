#include "WinnerView.h"
#include "System.h"

Texture WinnerView::texture = Texture();
bool WinnerView::texturesLoaded = false;

WinnerView::WinnerView(DirectX::SimpleMath::Vector2 position) : GuiElement(position)
{
	if (!WinnerView::texturesLoaded)
	{
		texture.setTexture("winner.tga");

		WinnerView::texturesLoaded = true;
	}
}

WinnerView::~WinnerView()
{
}

bool WinnerView::render(bool selected)
{
	System::getSpriteBatch()->Draw(texture.getTexture(), this->position);

	return false;
}
