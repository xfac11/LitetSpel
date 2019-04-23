#include "Button.h"

Button::Button(std::string texture, std::string text, Vector2 position, Vector2 size) : GuiElement(position), text(text), size(size)
{
	this->texture.setTexture(texture);
}

Button::~Button()
{
}

//bool Button::update(float deltaTime)
//{
//	//TODO
//	return true;
//}
//
//bool Button::render(DirectX::SpriteBatch* spriteBatch)
//{
//	//TODO
//	return true;
//}

ID3D11ShaderResourceView * Button::getTexture()
{
	return this->texture.getTexture();
}

Vector2 Button::getSize() const
{
	return this->size;
}

std::string Button::getText() const
{
	return this->text;
}

DirectX::SimpleMath::Rectangle Button::getRect() const
{
	return DirectX::SimpleMath::Rectangle(static_cast<long>(position.x), static_cast<long>(position.y), static_cast<long>(size.x), static_cast<long>(size.y));
}
