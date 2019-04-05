#include "BetterParticles.h"

bool BetterParticles::loadTexture(ID3D11Device *& device, ID3D11DeviceContext *& deviceContext, std::string filename)
{
	theTexture.setTexture(device, deviceContext, filename);
	return true;
}

void BetterParticles::initSystem()
{
	this->particleDeviation = { 2.5f,3.f,2.5f };
	this->particleVelocity = 10.f;
	this->particleVelocityVariation = 4.f;
	this->particleSize = 0.3f;
	this->particlesPerSecond = 2.0f;
	this->maxParticles = 100.f;

	this->currentParticleCount = 0;
	this->accumulatedTime =0.f;

	this->particleList = new ParticleType[this->maxParticles];
	for (int i = 0; i < this->maxParticles; i++)// Initialize the particle list.
	{
		this->particleList[i].active = false;
	}
}

bool BetterParticles::initBuffers(ID3D11Device * device)
{
	this->vertexCount = this->maxParticles * 6;

	this->indexCount = this->vertexCount;

	// Create the vertex array for the particles that will be rendered.
	//this->vertices = new Vertex3D[this->vertexCount];
	//this->body.capacity = this->vertexCount;
	this->body.resize(this->vertexCount);

	// Create the index array.
	unsigned long* indices = new unsigned long[this->indexCount];


	// Initialize the index array.
	for (int i = 0; i < this->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex3D) * this->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = this->body.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	HRESULT result;
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &this->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * this->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &this->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] indices;
	return true;
}

BetterParticles::BetterParticles()
{
	this->vertexBuffer = nullptr;
	this->indexBuffer = nullptr;
	this->SamplerState = nullptr;

	this->initSystem();
	this->currentParticleCount++;


	//std::vector<Vertex3D> body;

}

BetterParticles::~BetterParticles()
{
	if (this->particleList)
	{
		delete[] particleList;
		this->particleList = nullptr;
	}
	if (this->vertexBuffer)
	{
		this->vertexBuffer->Release();
		this->vertexBuffer = nullptr;
	}
	if (this->indexBuffer)
	{
		this->indexBuffer->Release();
		this->indexBuffer = nullptr;
	}
	if (this->SamplerState)
	{
		this->SamplerState->Release();
		this->SamplerState = nullptr;
	}
	theTexture.cleanUp();
}

bool BetterParticles::initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::string textureName)
{
	bool result = loadTexture(device, deviceContext, textureName);
	if (!result)
	{
		return false;
	}

	result = initBuffers(device);
	if (!result)
	{
		return false;
	}
	return true;
}
