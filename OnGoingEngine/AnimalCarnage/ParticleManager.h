#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include <vector>
#include "EffectBase.h"
#include <d3d11.h>
#include "SimpleMath.h"

using namespace DirectX;

class ParticleManager
{
private:
	std::vector<EffectBase*> activeEffects;

public:
	ParticleManager();
	virtual ~ParticleManager();

	void addEffect(SimpleMath::Vector3 position);

	void resetAndClear();
	void render();
	void update(float deltaTime);
};

#endif // !PARTICLE_MANAGER_H
