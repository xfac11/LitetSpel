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
	System::getFontArial()->DrawString(System::getSpriteBatch(), this->text.c_str(), this->position + Vector2(300, 50), selected ? DirectX::Colors::DarkGray : DirectX::Colors::Black, 0.0f, System::getFontArial()->MeasureString(this->text.c_str()) / 2.f, Vector2::One / 4 * 3);
	return true;
}
