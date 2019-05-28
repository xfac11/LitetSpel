#include "LooserView.h"
#include "System.h"

Texture LooserView::texture = Texture();
bool LooserView::texturesLoaded = false;

LooserView::LooserView(DirectX::SimpleMath::Vector2 position) : GuiElement(position)
{
	if (!LooserView::texturesLoaded)
	{
		texture.setTexture("selector.tga");

		LooserView::texturesLoaded = true;
	}
}

LooserView::~LooserView()
{
}

bool LooserView::render(bool selected)
{
	System::getSpriteBatch()->Draw(texture.getTexture(), this->position);

	return false;
}
