#include "CheckBox.h"

CheckBox::CheckBox(std::string texture, std::string checkTexture, bool checked, DirectX::SimpleMath::Vector2 position) : GuiElement(position)
{
	this->texture.setTexture(texture);
	this->checkTexture.setTexture(checkTexture);
	this->checked = checked;
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
	return this->texture.getTexture();
}

ID3D11ShaderResourceView * CheckBox::getCheckTexture()
{
	return this->checkTexture.getTexture();
}
