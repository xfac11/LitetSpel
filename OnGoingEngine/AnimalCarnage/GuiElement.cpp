#include "GuiElement.h"

GuiElement::GuiElement(DirectX::SimpleMath::Vector2 position) : position(position), left(nullptr), right(nullptr), up(nullptr), down(nullptr)
{
}

GuiElement::~GuiElement()
{
}

bool GuiElement::render(bool selected)
{
	return true;
}

void GuiElement::setConnectedElements(GuiElement * left, GuiElement * right, GuiElement * up, GuiElement * down)
{
	this->left = left;
	this->right = right;
	this->up = up;
	this->down = down;
}

void GuiElement::setPosition(DirectX::SimpleMath::Vector2 position)
{
	this->position = position;
}

DirectX::SimpleMath::Vector2 GuiElement::getPosition() const
{
	return this->position;
}

GuiElement * GuiElement::getLeft() const
{
	return this->left;
}

GuiElement * GuiElement::getRight() const
{
	return this->right;
}

GuiElement * GuiElement::getUp() const
{
	return this->up;
}

GuiElement * GuiElement::getDown() const
{
	return this->down;
}
