#include "Sprite.h"
#include "System.h"

Sprite::Sprite(std::string textureFile)
{
	this->texture = new Texture();
	this->texture->setTexture(textureFile);
}

Sprite::~Sprite()
{
}

Texture * Sprite::getTexture()
{
	return this->texture;
}

DirectX::SimpleMath::Vector2 Sprite::getPosition()
{
	return this->m_screenPos;
}

DirectX::SimpleMath::Vector2 Sprite::getOrigin()
{
	return this->m_origin;
}
