#include "SimpleEffect.h"

Texture* SimpleEffect::particleTexture = nullptr;

SimpleEffect::SimpleEffect(SimpleMath::Vector3 position) : EffectBase(), position(position)
{
	if (!SimpleEffect::particleTexture)
	{
		SimpleEffect::particleTexture->setTexture("cat.tga");
	}
}

SimpleEffect::~SimpleEffect()
{
}

void SimpleEffect::update(float deltaTime)
{
}

void SimpleEffect::render()
{
}