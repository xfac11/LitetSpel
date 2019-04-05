#ifndef BETTERPARTICLES_H
#define BETTERPARTICLES_H
#include <directxmath.h>
#include <string>
#include <vector>
#include "Texture.h"
#include "Vertex3D.h"
#include "colorShader.h" //make custom made shader for billboard
class BetterParticles
{
private:
	struct ParticleType
	{
		DirectX::XMFLOAT3 position;
		//DirectX::XMFLOAT3 rgb;
		float velocity;
		bool active;
		//add padding?
	};
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

	std::vector<Vertex3D> body;
	ID3D11Buffer *vertexBuffer;
	int vertexCount;
	ID3D11Buffer *indexBuffer;
	int indexCount;
	ID3D11SamplerState* SamplerState;


	bool loadTexture(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, std::string filename);
	void initSystem();
	bool initBuffers(ID3D11Device* device);
public:
	BetterParticles();
	~BetterParticles();
	bool initialize(ID3D11Device * device, ID3D11DeviceContext* deviceContext, std::string textureName);

	
	void prepareBuffers();
	void prepare(unsigned int& shaderProg); //Prepare for render

	
	
	void update(double deltaTime); //frameTime or deltaTime??
};
#endif