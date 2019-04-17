/*#ifndef SPRITE_H
#define SPRITE_H

#include "Texture.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include <CommonStates.h>
using namespace  DirectX;
using namespace SimpleMath;

class Sprite
{
private:
	Texture sprite;
	Vector2 position;
	Vector2 origin;
	float scale;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::CommonStates> spriteStates;
public:
	Sprite(std::string textureFile, float scale = 1.0f,Vector2 pos  =Vector2(0,0));
	~Sprite();

	bool Render();
	ID3D11ShaderResourceView* getTexture();
	DirectX::SimpleMath::Vector2 getPosition();
	DirectX::SimpleMath::Vector2 getOrigin();
};

#endif // !SPRITE_H*/
