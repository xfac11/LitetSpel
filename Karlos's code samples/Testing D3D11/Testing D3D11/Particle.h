#ifndef PARTICLE_H
#define PARTICLE_H
#include <d3d11.h>
#include <directxmath.h>
#include "Texture.h"
#include "Vertex3D.h"
#include "colorShader.h"
class Particle
{
private:
	struct ParticleType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 rgb;
		float velocity;
		bool active;
	};
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 Rotation;
	DirectX::XMFLOAT4X4 Scale;
	DirectX::XMFLOAT4X4 Translation;
	DirectX::XMFLOAT3 position;

	ParticleType properties;

public:
	Particle();
	~Particle();
	void Shutdown();
	
	void setPosition(DirectX::XMFLOAT3 position);
	void setY(float y);
	DirectX::XMFLOAT3 getPosition();

	ParticleType & getParticle();
	void setWorld();
	DirectX::XMFLOAT4X4 getWorld();
	void billboard(DirectX::XMFLOAT3 camPos);
	
};
#endif