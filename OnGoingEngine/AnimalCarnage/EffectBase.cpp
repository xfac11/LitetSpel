#include "EffectBase.h"

EffectBase::EffectBase(float lifeTime) : aliveForever(false), lifetime(lifeTime), currentAge(0)
{
}

EffectBase::EffectBase(bool aliveForever, float lifeTime) : aliveForever(aliveForever), lifetime(lifeTime), currentAge(0)
{
}

EffectBase::~EffectBase()
{
}

void EffectBase::update(float deltaTime)
{
	this->currentAge += deltaTime;
}

void EffectBase::render()
{
}

bool EffectBase::isAlive() const
{
	return this->currentAge < this->lifetime || this->aliveForever;
}
