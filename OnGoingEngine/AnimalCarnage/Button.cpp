#include "Button.h"
#include "System.h"

bool Button::texturesLoaded = false;
Texture Button::texture = Texture();
Texture Button::textureSelected = Texture();

Button::Button(std::string text, Vector2 position) : GuiElement(position), text(text)
{
	if (!Button::texturesLoaded)
	{
		Button::texture.setTexture("button.tga");
		Button::textureSelected.setTexture("buttonSelected.tga");
		Button::texturesLoaded = true;
	}
}

Button::~Button()
{
}

bool Button::render(bool selected)
{
	System::getSpriteBatch()->Draw(selected ? Button::textureSelected.getTexture() : Button::texture.getTexture(), this->position, nullptr);
	System::getFontComicSans()->DrawString(System::getSpriteBatch(), this->text.c_str(), this->position, DirectX::Colors::Black, 0.0f, Vector2::Zero, Vector2::One * 3);
	return true;
}
