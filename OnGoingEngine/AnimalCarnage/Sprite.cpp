#include "Sprite.h"
#include "System.h"

Sprite::Sprite(std::string textureFile, float scale, Vector2 pos)
{
	//TODO
	spriteStates = std::make_unique<CommonStates>(System::getDevice());
	spriteBatch = std::make_unique<SpriteBatch>(System::getDeviceContext());

	sprite.setTexture(textureFile);
	this->position = pos;
	this->scale = scale;
	this->origin = DirectX::SimpleMath::Vector2(this->sprite.getWidth() / 2, this->sprite.getHeight() / 2);

}

Sprite::~Sprite()
{

	/*if (this->sprite != nullptr)
	{
		this->sprite.cleanUp();
		delete this->sprite;
	}*/


}

bool Sprite::Render()
{

	spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, spriteStates->NonPremultiplied());
	spriteBatch->Draw(this->sprite.getTexture(), position,nullptr,Colors::White,0.0f,origin, scale);
	spriteBatch->End();

	auto samplerState = spriteStates->LinearWrap();
	System::getDeviceContext()->PSSetSamplers(0, 1, &samplerState);
	System::getDeviceContext()->OMSetBlendState(spriteStates->Opaque(), Colors::OrangeRed, 0xFFFFFFFF);
	System::getDeviceContext()->OMSetDepthStencilState(spriteStates->DepthDefault(), 0);
	System::getDeviceContext()->RSSetState(spriteStates->CullNone());

	return true;
}
ID3D11ShaderResourceView* Sprite::getTexture()
{
	return sprite.getTexture();
}

DirectX::SimpleMath::Vector2 Sprite::getPosition()
{
	return this->position;
}

DirectX::SimpleMath::Vector2 Sprite::getOrigin()
{
	return this->origin;
}
