#include "BetterParticles.h"

bool BetterParticles::loadTexture(ID3D11Device *& device, ID3D11DeviceContext *& deviceContext, std::string filename)
{
	theTexture.setTexture(device, deviceContext, filename);
	return true;
}

void BetterParticles::initSystem()
{

	this->particleDeviation = { 2.5f,3.f,2.5f }; //in this
	this->particleVelocity = 10.f; //in computeShader
	this->particleVelocityVariation = 4.f; //in this
	this->particleSize = 0.3f; //in geometry shader
	this->particlesPerSecond = 2.0f; //in this
	this->maxParticles = 100;  //in this

	this->currentParticleCount = 0;
	this->accumulatedTime =0.f;

	this->body.resize(this->maxParticles);
	this->data = new Compute;
	this->particleList = new ParticleType[this->maxParticles];
	for (int i = 0; i < this->maxParticles; i++)// Initialize the particle list.
	{
		

		float x = (((float)rand() - (float)rand()) / RAND_MAX) * this->particleDeviation.x;
		float y = (((float)rand() - (float)rand()) / RAND_MAX) * this->particleDeviation.y;
		float z = (((float)rand() - (float)rand()) / RAND_MAX) * this->particleDeviation.z;

		float velocity = this->particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * this->particleVelocityVariation;


		this->particleList[i].position = { x,y,z };
		this->particleList[i].direction = { 0.f, 1.f, 0.f };
		this->particleList[i].velocity = velocity;
		this->particleList[i].active = false; //true


		this->body.at(i).position.x = this->particleList[i].position.x;
		this->body.at(i).position.y = this->particleList[i].position.y;
		this->body.at(i).position.z = this->particleList[i].position.y;

	}


}

bool BetterParticles::initBuffers(ID3D11Device * device)
{
	this->vertexCount = this->maxParticles * 4;

	//this->indexCount = this->vertexCount;

	// Create the vertex array for the particles that will be rendered.
	//this->vertices = new Vertex3D[this->vertexCount];
	//this->body.capacity = this->vertexCount;
	//this->body.resize(this->vertexCount);

	// Create the index array.
	//unsigned long* indices = new unsigned long[this->indexCount];

	// Initialize the index array.
	//for (int i = 0; i < this->indexCount; i++)
	//{
	//	indices[i] = i;
	//}

	// Set up the description of the dynamic vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(ParticleInput) *maxParticles;// this->vertexCount;
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
	//D3D11_BUFFER_DESC indexBufferDesc;
	//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexBufferDesc.ByteWidth = sizeof(unsigned long) * this->indexCount;
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;
	//indexBufferDesc.StructureByteStride = 0;

	//// Give the subresource structure a pointer to the index data.
	//D3D11_SUBRESOURCE_DATA indexData;
	//indexData.pSysMem = indices;
	//indexData.SysMemPitch = 0;
	//indexData.SysMemSlicePitch = 0;

	//// Create the index buffer.
	//result = device->CreateBuffer(&indexBufferDesc, &indexData, &this->indexBuffer);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	//delete[] indices;


	
	//compute data for deltaTime in computeshader
	D3D11_BUFFER_DESC dataDesc;
	dataDesc.Usage = D3D11_USAGE_DYNAMIC;//D3D11_USAGE_DEFAULT;
	dataDesc.ByteWidth = sizeof(Compute)*4;
	dataDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;// D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;// D3D11_BIND_CONSTANT_BUFFER;
	dataDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	dataDesc.MiscFlags = 0;
	dataDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA cdData;//
	cdData.pSysMem = data;
	cdData.SysMemPitch = 0;
	cdData.SysMemSlicePitch = 0;
	result = device->CreateBuffer(&dataDesc, &cdData,&dataBuffer);



	return true;
}

BetterParticles::BetterParticles()
{
	this->vertexBuffer = nullptr;
	//this->indexBuffer = nullptr;
	this->SamplerState = nullptr;
	this->dataBuffer = nullptr;
	this->initSystem();
	this->currentParticleCount++;


	//std::vector<Vertex3D> body;


	DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMMATRIX rotTemp = DirectX::XMMatrixRotationAxis(rotaxis, 0);
	DirectX::XMMATRIX scaleTemp = DirectX::XMMatrixScaling(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX translTemp = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&this->Rotation, rotTemp);
	DirectX::XMStoreFloat4x4(&this->Scale, scaleTemp);
	DirectX::XMStoreFloat4x4(&this->Translation, translTemp);
	this->position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scaleTemp = DirectX::XMLoadFloat4x4(&this->Scale);
	translTemp = DirectX::XMLoadFloat4x4(&this->Translation);




}

BetterParticles::~BetterParticles()
{

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

void BetterParticles::shutdown()
{	
	if (this->particleList)
	{
		delete[] particleList;
		this->particleList = nullptr;
	}
	if (this->data)
	{
		delete this->data;
	}

	if (this->dataBuffer)
	{
		this->dataBuffer->Release();
		this->dataBuffer = nullptr;
	}

	if (this->vertexBuffer)
	{
		this->vertexBuffer->Release();
		this->vertexBuffer = nullptr;
	}
	//if (this->indexBuffer)
	//{
	//	this->indexBuffer->Release();
	//	this->indexBuffer = nullptr;
	//}
	if (this->SamplerState)
	{
		this->SamplerState->Release();
		this->SamplerState = nullptr;
	}
	theTexture.cleanUp();
}

void BetterParticles::draw(ParticleShader & shader, ID3D11DeviceContext * deviceContext)
{
	unsigned int stride = sizeof(ParticleInput);
	unsigned int offset = 0;
	deviceContext->PSSetShaderResources(0, 1, &this->theTexture.getTexture());
	deviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
	//deviceContext->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	deviceContext->PSSetSamplers(0, 1, &this->SamplerState);

	shader.RenderShader(deviceContext,maxParticles);
}

bool BetterParticles::updateBuffers(ID3D11Device * device, ID3D11DeviceContext * deviceContext, float deltaTime) //send to every frame shaders
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	int index = 0;
	//Vertex3D* verticesPtr;

	//for (int i = 0; i < this->currentParticleCount; i++)
	//{
	//	this->body.at(index).x= this->particleList[i].position.x - this->particleSize;
	//	//pos
	//	//tex
	//	//color
	//	
	//}


	//result = deviceContext->Map(this->particleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	//if (FAILED(result))
	//{
	//	return false;
	//}
	//// Get a pointer to the data in the constant buffer.
	//memcpy(mappedMemory.pData, particleList, sizeof(ParticleType)*this->maxParticles);
	//// Unlock the constant buffer.
	//deviceContext->Unmap(this->particleBuffer, 0);


	D3D11_BUFFER_DESC particlesDesc;
	memset(&particlesDesc, 0, sizeof(particlesDesc));

	particlesDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	particlesDesc.ByteWidth = sizeof(ParticleType)*this->maxParticles;
	particlesDesc.CPUAccessFlags = 0;
	particlesDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	particlesDesc.StructureByteStride = sizeof(ParticleType);
	particlesDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Buffer* particles_ptr = nullptr;
	device->CreateBuffer(&particlesDesc, nullptr, &particles_ptr);
	if (!particles_ptr)
		return false;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	uav_desc.Format = DXGI_FORMAT_UNKNOWN;// DXGI_FORMAT_R32_TYPELESS;//DXGI_FORMAT_R32G32B32_UINT; //DXGI_FORMAT_UNKNOWN
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uav_desc.Buffer.FirstElement = 0;
	
	uav_desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;// D3D11_BUFFER_UAV_FLAG_RAW;//0;
	uav_desc.Buffer.NumElements = this->maxParticles; //100elements?? this->nrOf

	ID3D11UnorderedAccessView* particle_uav = nullptr;
	result = device->CreateUnorderedAccessView(particles_ptr, &uav_desc, &particle_uav);

	//computer shader
	//float3 position;
	//float3 direction;
	//float velocity;
	//bool active; 

	//computer shader
	data->deltaTime = deltaTime;
	result = deviceContext->Map(this->dataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	if (FAILED(result))
	{
		return false;
	}
	// Get a pointer to the data in the constant buffer.
	memcpy(mappedMemory.pData, data, sizeof(Compute));
	// Unlock the constant buffer.
	deviceContext->Unmap(this->dataBuffer, 0);

	deviceContext->CSSetConstantBuffers(0, 1, &this->dataBuffer);
	deviceContext->CSSetUnorderedAccessViews(0,1, &particle_uav,nullptr); //if crash change nullptr to current particle id
	//https://docs.microsoft.com/sr-latn-rs/windows/desktop/api/d3d11/nf-d3d11-id3d11devicecontext-cssetunorderedaccessviews

	this->update(deltaTime);

	return true;
}

void BetterParticles::update(float deltaTime) //emits?
{
	this->accumulatedTime += deltaTime;
	if (this->currentParticleCount < maxParticles-1 &&
		this->accumulatedTime*this->particlesPerSecond >= 1.f)
	{

		//position.x = (((float)rand() - (float)rand()) / RAND_MAX) * this->particleDeviation.x;
		//position.y = (((float)rand() - (float)rand()) / RAND_MAX) * this->particleDeviation.y;
		//position.z = (((float)rand() - (float)rand()) / RAND_MAX) * this->particleDeviation.z;
		this->particleList[this->currentParticleCount].active = true;
		this->currentParticleCount++; //adds
		this->accumulatedTime = 0.f;
	}
}

void BetterParticles::setWorld()
{
	DirectX::XMMATRIX rotTemp;
	DirectX::XMMATRIX scaleTemp;
	DirectX::XMMATRIX translTemp;
	rotTemp = DirectX::XMLoadFloat4x4(&this->Rotation);
	scaleTemp = DirectX::XMLoadFloat4x4(&this->Scale);
	translTemp = DirectX::XMLoadFloat4x4(&this->Translation);
	DirectX::XMStoreFloat4x4(&this->world, (rotTemp*translTemp));
}

DirectX::XMFLOAT4X4 BetterParticles::getWorld()
{
	return this->world;
}

void BetterParticles::setPosition(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	DirectX::XMMATRIX tempTransl = DirectX::XMMatrixTranslation(x, y, z);
	DirectX::XMStoreFloat4x4(&this->Translation, tempTransl);
}

void BetterParticles::setSampler(ID3D11Device *& gDevice)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = gDevice->CreateSamplerState(&desc, &this->SamplerState);
	if (FAILED(hr))
	{
		//		MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", "error", MB_OK);
	}
}
