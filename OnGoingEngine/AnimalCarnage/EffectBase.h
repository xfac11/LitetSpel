#ifndef EFFECT_BASE_H
#define EFFECT_BASE_H

#include <d3d11.h>
#include "SimpleMath.h"

using namespace DirectX;

class EffectBase
{
private:
	float lifetime;
	float currentAge;
	bool aliveForever;
public:
	EffectBase(float lifeTime = 10.0f);
	EffectBase(bool aliveForever, float lifeTime = 0.0f);
	virtual ~EffectBase();

	virtual void update(float deltaTime);
	virtual void render();

	bool isAlive() const;
};

#endif // !EFFECT_BASE_H
