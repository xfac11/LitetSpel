#pragma once
#include "Texture.h"
#include "SimpleMath.h"

class Sprite
{
private:
	Texture texture;
	DirectX::SimpleMath::Vector2 m_screenPos;
	DirectX::SimpleMath::Vector2 m_origin;
public:
	Sprite(std::string textureFile);
	~Sprite();

	Texture* getTexture();
	DirectX::SimpleMath::Vector2 getPosition();
	DirectX::SimpleMath::Vector2 getOrigin();
};

