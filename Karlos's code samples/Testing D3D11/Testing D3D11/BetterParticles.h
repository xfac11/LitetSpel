#ifndef BETTERPARTICLES_H
#define BETTERPARTICLES_H
#include <directxmath.h>
#include <string>
#include <vector>
#include "Texture.h"
#include "Vertex3D.h"
#include "ParticleShader.h" //make custom made shader for billboard
class BetterParticles
{
private:
	struct ParticleInput
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 color;
	};

	struct ParticleType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 direction;
		float velocity;
		bool active;
		//add padding?
	};
	struct Compute
	{
		float deltaTime;
	};

	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 Rotation;
	DirectX::XMFLOAT4X4 Scale;
	DirectX::XMFLOAT4X4 Translation;
	DirectX::XMFLOAT3 position;

	DirectX::XMFLOAT3 particleDeviation;
	float particleVelocity;
	float particleVelocityVariation;
	float particleSize;
	float particlesPerSecond; //emitRate
	int maxParticles;

	int currentParticleCount;
	float accumulatedTime;

	Texture theTexture;

	ParticleType* particleList;
	//ID3D11Buffer* particleBuffer;
	Compute* data;
	ID3D11Buffer* dataBuffer;

	std::vector<ParticleInput> body;
	ID3D11Buffer *vertexBuffer;
	int vertexCount;
	//ID3D11Buffer *indexBuffer;
	//int indexCount;
	ID3D11SamplerState* SamplerState;


	bool loadTexture(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, std::string filename);
	void initSystem();
	bool initBuffers(ID3D11Device* device);
	void update(float deltaTime);
public:
	BetterParticles();
	~BetterParticles();
	bool initialize(ID3D11Device * device, ID3D11DeviceContext* deviceContext, std::string textureName);
	bool updateBuffers(ID3D11Device * device,ID3D11DeviceContext * deviceContext, float deltaTime);
	void shutdown();
	void draw(ParticleShader & shader, ID3D11DeviceContext* deviceContext);
	


	void setWorld();
	DirectX::XMFLOAT4X4 getWorld();
	void setPosition(float x, float y, float z);
	void setSampler(ID3D11Device *& gDevice);
};
#endif