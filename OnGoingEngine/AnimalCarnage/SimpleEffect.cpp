#include "SimpleEffect.h"
#include "System.h"

SimpleEffect::SimpleEffect(SimpleMath::Vector3 position, float lifeTime,bool gravity, int particleCount, float size, float maxStartSpeed, std::string fileName) : EffectBase(lifeTime)
{
	this->particles = new Particle[particleCount];
	this->velocities = new SimpleMath::Vector3[particleCount];
	this->particleCount = particleCount;
	this->gravity = gravity;
	for (int i = 0; i < particleCount; i++)
	{
		this->particles[i].position = position;
		//this->particles[i].size = size;
		this->velocities[i] = SimpleMath::Vector3((rand() % 1001 - 500) / 500.0f, (rand() % 1001 - 500) / 500.0f, (rand() % 1001 - 500) / 500.0f);
		this->velocities[i] *= maxStartSpeed;
	}
	System::shaderManager->getParticleShader()->setSize(size);
	this->vertexBuffer.initializeDynamic(this->particles, static_cast<UINT>(particleCount), System::getDevice());
	this->theShader = System::shaderManager->getParticleShader();

	System::assetMananger->LoadTexture(fileName, fileName + ".tga");
	this->particleTexture = System::assetMananger->GetTexture(fileName);

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = System::getDevice()->CreateSamplerState(&desc, &this->SamplerState);
	if (FAILED(hr))
	{
		//deal with error. Log it maybe
	}
}

SimpleEffect::~SimpleEffect()
{
	delete[] this->particles;
	delete[] this->velocities;

	this->particleTexture = nullptr;
	this->SamplerState->Release();
}

void SimpleEffect::update(float deltaTime)
{
	EffectBase::update(deltaTime);

	for (int i = 0; i < this->particleCount; i++)
	{
		//F_res = m * g
		//a = F_res / m
		//a = g
		//v = v0 + a * t
		//s = v * t
		this->velocities[i] = this->velocities[i]*0.5f + 9.82f * this->gravity * deltaTime * SimpleMath::Vector3::Down;
		this->particles[i].position = this->particles[i].position + this->velocities[i] * deltaTime;
	}

	this->vertexBuffer.applyChanges(System::getDevice(), System::getDeviceContext());
}

void SimpleEffect::render()
{
	UINT32 offset = 0;
	System::getDeviceContext()->PSSetShaderResources(0, 1, &this->particleTexture->getTexture());
	System::getDeviceContext()->IASetVertexBuffers(0, 1, &*this->vertexBuffer.GetAddressOf(), &*vertexBuffer.getStridePtr(), &offset);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	System::getDeviceContext()->PSSetSamplers(0, 1, &this->SamplerState);

	this->theShader->renderShader(this->particleCount, 0);
}