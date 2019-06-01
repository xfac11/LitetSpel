#ifndef SIMPLE_EFFECT_H
#define SIMPLE_EFFECT_H

#include "EffectBase.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "Shader.h"

class SimpleEffect : public EffectBase
{
private:
	struct Particle
	{
		XMFLOAT3 position;
		float size;
	};

	ID3D11SamplerState* SamplerState;
	VertexBuffer<Particle> vertexBuffer;
	Shader* theShader;
	std::shared_ptr<Texture> particleTexture;
	bool gravity;
	int particleCount;
	SimpleMath::Vector3* velocities;
	SimpleMath::Vector3 movingSpeed;
	Particle* particles;
public:
	SimpleEffect(SimpleMath::Vector3 position = SimpleMath::Vector3(), float lifeTime = 10.0f
		,bool gravity=true, int particleCount = 10,float size=1.0f, float maxStartSpeed = 5.0f, std::string fileName = "", 
		SimpleMath::Vector3 movingSpeed = SimpleMath::Vector3(0,0,0));
	virtual ~SimpleEffect();

	virtual void update(float deltaTime);
	virtual void render();
};

#endif // !SIMPLE_EFFECT_H
