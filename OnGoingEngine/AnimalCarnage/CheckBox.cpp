#include "CheckBox.h"
#include "System.h"

bool CheckBox::texturesLoaded = false;
Texture CheckBox::texture = Texture();
Texture CheckBox::textureSelected = Texture();
Texture CheckBox::checkTexture = Texture();

CheckBox::CheckBox(bool checked, DirectX::SimpleMath::Vector2 position) : GuiElement(position), checked(checked)
{
	if (!CheckBox::texturesLoaded)
	{
		CheckBox::texture.setTexture("check.tga");
		CheckBox::textureSelected.setTexture("checkSelected.tga");
		CheckBox::checkTexture.setTexture("checkBG.tga");

		CheckBox::texturesLoaded = true;
	}
}

CheckBox::~CheckBox()
{
}

bool CheckBox::render(bool selected)
{
	System::getSpriteBatch()->Draw(selected ? CheckBox::textureSelected.getTexture() : CheckBox::texture.getTexture(), this->position, nullptr);

	if (this->checked)
	{
		System::getSpriteBatch()->Draw(CheckBox::checkTexture.getTexture(), this->position, nullptr);
	}

	return true;
}

void CheckBox::setChecked(bool isChecked)
{
	this->checked = isChecked;
}

bool CheckBox::isChecked() const
{
	return this->checked;
}
