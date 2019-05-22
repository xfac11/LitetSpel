#ifndef SIMPLE_EFFECT_H
#define SIMPLE_EFFECT_H

#include "EffectBase.h"
#include "Texture.h"

class SimpleEffect : public EffectBase
{
private:
	static Texture* particleTexture;
	SimpleMath::Vector3 position;

public:
	SimpleEffect(SimpleMath::Vector3 position = SimpleMath::Vector3());
	virtual ~SimpleEffect();

	virtual void update(float deltaTime);
	virtual void render();
};

#endif // !SIMPLE_EFFECT_H
