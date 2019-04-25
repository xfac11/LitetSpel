#include "CheckBox.h"

bool CheckBox::texturesLoaded = false;
Texture CheckBox::texture = Texture();
Texture CheckBox::checkTexture = Texture();

CheckBox::CheckBox(bool checked, DirectX::SimpleMath::Vector2 position) : GuiElement(position), checked(checked)
{
	if (!CheckBox::texturesLoaded)
	{
		CheckBox::texture.setTexture("check.tga");
		CheckBox::checkTexture.setTexture("checkBG.tga");

		CheckBox::texturesLoaded = true;
	}
}

CheckBox::~CheckBox()
{
}

void CheckBox::setChecked(bool isChecked)
{
	this->checked = isChecked;
}

bool CheckBox::isChecked() const
{
	return this->checked;
}

ID3D11ShaderResourceView * CheckBox::getTexture()
{
	return CheckBox::texture.getTexture();
}

ID3D11ShaderResourceView * CheckBox::getCheckTexture()
{
	return CheckBox::checkTexture.getTexture();
}
