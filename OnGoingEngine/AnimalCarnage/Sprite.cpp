#include "Sprite.h"
#include "System.h"

Sprite::Sprite(std::string textureFile)
{
	//TODO
	spriteStates = std::make_unique<CommonStates>(System::getDevice());

	m_spriteBatch = std::make_unique<SpriteBatch>(System::getDeviceContext());

	sprite.setTexture(textureFile);
	this->m_screenPos = DirectX::SimpleMath::Vector2(0, 0);
	this->origin = DirectX::SimpleMath::Vector2(this->sprite.getWidth() / 2, this->sprite.getHeight() / 2);
}

Sprite::~Sprite()
{

	//if (this->sprite != nullptr)
	//{
	//	this->sprite->cleanUp();
	//	delete this->sprite;
	//}

}

bool Sprite::Render()
{
	

	m_spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, spriteStates->NonPremultiplied());
	m_spriteBatch->Draw(this->sprite.getTexture(), m_screenPos);
	m_spriteBatch->End();

	auto samplerState = spriteStates->LinearWrap();
	System::getDeviceContext()->PSSetSamplers(0, 1, &samplerState);
	System::getDeviceContext()->OMSetBlendState(spriteStates->Opaque(), Colors::OrangeRed, 0xFFFFFFFF);
	System::getDeviceContext()->OMSetDepthStencilState(spriteStates->DepthDefault(), 0);
	System::getDeviceContext()->RSSetState(spriteStates->CullNone());

	return false;
}
ID3D11ShaderResourceView* Sprite::getTexture()
{
	return sprite.getTexture();
}

DirectX::SimpleMath::Vector2 Sprite::getPosition()
{
	return this->m_screenPos;
}

DirectX::SimpleMath::Vector2 Sprite::getOrigin()
{
	return this->origin;
}
