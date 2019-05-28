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

	void addSimpleEffect(SimpleMath::Vector3 position = SimpleMath::Vector3(1,1,1), std::string filename = "splat", float size=1.0f, float lifetime=5.0f, int particleCount=5.0f, float maxStartSpeed=5.0f);

	void resetAndClear();
	void render();
	void update(float deltaTime);
};

#endif // !PARTICLE_MANAGER_H
