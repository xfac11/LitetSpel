#ifndef SPRITE_H
#define SPRITE_H
#include "Texture.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include <CommonStates.h>

class Sprite
{
private:
	Texture sprite;
	DirectX::SimpleMath::Vector2 m_screenPos;
	DirectX::SimpleMath::Vector2 origin;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::CommonStates> spriteStates;
public:
	Sprite(std::string textureFile);
	~Sprite();

	bool Render();
	ID3D11ShaderResourceView* getTexture();
	DirectX::SimpleMath::Vector2 getPosition();
	DirectX::SimpleMath::Vector2 getOrigin();
};

#endif // !SPRITE_H
