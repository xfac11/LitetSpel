#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() 
{
	
	particleList = nullptr;
	//vertices = nullptr;
	this->vertexBuffer = nullptr;
	this->indexBuffer = nullptr;
	this->SamplerState = nullptr;

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

ParticleSystem::~ParticleSystem()
{
}

bool ParticleSystem::Initialize(ID3D11Device * device, ID3D11DeviceContext* deviceContext, std::string textureName, std::string normalFileName)
{ 
	bool result;

	// Load the texture that is used for the particles.
	result = LoadTexture(device, deviceContext, textureName,normalFileName);
	if (!result)
	{
		return false;
	}

	// Initialize the particle system.
	result = InitializeParticleSystem();
	if (!result)
	{
		return false;
	}

	// Create the buffers that will be used to render the particles with.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void ParticleSystem::Shutdown() //copied
{
	if (this->indexBuffer)
	{
		this->indexBuffer->Release();
		this->indexBuffer = nullptr;
	}
	if (this->vertexBuffer)
	{
		this->vertexBuffer->Release();
		this->vertexBuffer = nullptr;
	}
	if (this->SamplerState)
	{
		this->SamplerState->Release();
		this->SamplerState = nullptr;
	}
	if (this->particleList)
	{
		delete[] particleList;
		this->particleList = nullptr;
	}
	//if (this->vertices)
	//{
	//	delete[] vertices;
	//	this->vertices = nullptr;
	//}
	theTexture.cleanUp();
	normal.cleanUp();
}

bool ParticleSystem::Frame(float frameTime, ID3D11DeviceContext * deviceContext)
{ 
	bool result;
	KillParticles();// Release old particles.
	EmitParticles(frameTime);// Emit new particles.
	UpdateParticles(frameTime);// Update the position of the particles.
	result = UpdateBuffers(deviceContext);// Update the dynamic vertex buffer with the new position of each particle.
	if (!result)
	{
		return false;
	}
	return true;
}

void ParticleSystem::draw(DeferedShader & shader, ID3D11DeviceContext * deviceContext)
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(Vertex3D);
	unsigned int offset = 0;
	deviceContext->PSSetShaderResources(0, 1, &this->theTexture.getTexture());

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->PSSetSamplers(0, 1, &this->SamplerState);
	shader.RenderShader(deviceContext, indexCount);
}


void ParticleSystem::setWorld()
{
	DirectX::XMMATRIX rotTemp;
	DirectX::XMMATRIX scaleTemp;
	DirectX::XMMATRIX translTemp;
	rotTemp = DirectX::XMLoadFloat4x4(&this->Rotation);
	scaleTemp = DirectX::XMLoadFloat4x4(&this->Scale);
	translTemp = DirectX::XMLoadFloat4x4(&this->Translation);
	DirectX::XMStoreFloat4x4(&this->world, (translTemp));
}

DirectX::XMFLOAT4X4 ParticleSystem::getWorld()
{
	return this->world;
}

void ParticleSystem::billboard( DirectX::XMFLOAT3 camPos) //can do this in GS?
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX translateMatrix;

	double angle = atan2(position.x - camPos.x, position.z - camPos.z) * (180.0 / DirectX::XM_PI);
	float rotation = (float)angle * 0.0174532925f;

	worldMatrix = DirectX::XMLoadFloat4x4(&world);
	worldMatrix = DirectX::XMMatrixRotationY(rotation);
	translateMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translateMatrix);
	DirectX::XMStoreFloat4x4(&this->world, worldMatrix);
}

void ParticleSystem::setPosition(float x, float y, float z)  
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	DirectX::XMMATRIX tempTransl = DirectX::XMMatrixTranslation(x, y, z);
	DirectX::XMStoreFloat4x4(&this->Translation, tempTransl);
}

void ParticleSystem::setSampler(ID3D11Device *& gDevice) 
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

bool ParticleSystem::LoadTexture(ID3D11Device *& device, ID3D11DeviceContext*& deviceContext, std::string filename, std::string normalFileName)
{ 
	// Initialize the texture object.
	theTexture.setTexture(device, deviceContext, filename);
	normal.setTexture(device, deviceContext, normalFileName);
	return true;
}

bool ParticleSystem::InitializeParticleSystem() 
{
	// Set the random deviation of where the particles can be located when emitted.
	this->particleDeviation.x = 2.5f;
	this->particleDeviation.y = 3.f;
	this->particleDeviation.z = 2.5f;

	// Set the speed and speed variation of particles.
	this->particleVelocity = 10.f;
	this->particleVelocityVariation = 2.f;

	this->particleSize = 0.1f; 
	this->particlesPerSecond = 2.0f; 
	this->maxParticles = 100;
	this->particleList = new ParticleType[this->maxParticles];
	for (int i = 0; i < this->maxParticles; i++)// Initialize the particle list.
	{
		this->particleList[i].active = false;
	}
	this->currentParticleCount = 0;// Initialize the current particle count to zero since none are emitted yet.

	// Clear the initial accumulated time for the particle per second emission rate.
	this->accumulatedTime = 0.0f;
	return true;
}


bool ParticleSystem::InitializeBuffers(ID3D11Device * device) 
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;

	// Set the maximum number of vertices in the vertex array.
	this->vertexCount = this->maxParticles * 6;

	// Set the maximum number of indices in the index array.
	this->indexCount = this->vertexCount;

	// Create the vertex array for the particles that will be rendered.
	//this->vertices = new Vertex3D[this->vertexCount];
	//this->body.capacity = this->vertexCount;
	this->body.resize(this->vertexCount);

	// Create the index array.
	unsigned long* indices = new unsigned long[this->indexCount];

	// Initialize vertex array to zeros at first.
	//memset(this->vertices, 0, (sizeof(Vertex3D) * this->vertexCount));

	// Initialize the index array.
	for (int i = 0; i < this->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex3D) * this->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = this->body.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &this->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * this->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
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


void ParticleSystem::EmitParticles(float frameTime) 
{
	bool emitParticle;
	bool found;
	DirectX::XMFLOAT3 position;
	float velocity;
	DirectX::XMFLOAT3 rgb;
	int index;

	// Increment the frame time.
	this->accumulatedTime += frameTime;

	// Set emit particle to false for now.
	emitParticle = false;

	// Check if it is time to emit a new particle or not.
	if (this->accumulatedTime > (1.0f / this->particlesPerSecond))
	{
		this->accumulatedTime = 0.0f;
		emitParticle = true;
	}

	// If there are particles to emit then emit one per frame.
	if ((emitParticle == true) && (this->currentParticleCount < (this->maxParticles - 1)))
	{
		this->currentParticleCount++;

		// Now generate the randomized particle properties.
		position.x = (((float)rand() - (float)rand()) / RAND_MAX) * this->particleDeviation.x;
		position.y = (((float)rand() - (float)rand()) / RAND_MAX) * this->particleDeviation.y;
		position.z = (((float)rand() - (float)rand()) / RAND_MAX) * this->particleDeviation.z;

		velocity = this->particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * this->particleVelocityVariation;

		rgb.x = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		rgb.y = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		rgb.z = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

		// Now since the particles need to be rendered from back to front for blending we have to sort the particle array.
		// We will sort using Z depth so we need to find where in the list the particle should be inserted.
		index = 0;
		found = false;
		while (!found)
		{
			if ((this->particleList[index].active == false) || (this->particleList[index].position.z < position.z))
				found = true;
			else
				index++;
		}

		// Now that we know the location to insert into we need to copy the array over by one position from the index to make room for the new particle.
		int i = this->currentParticleCount;
		int j = i - 1;

		while (i != index)
		{
			this->particleList[i].position.x = this->particleList[j].position.x;
			this->particleList[i].position.y = this->particleList[j].position.y;
			this->particleList[i].position.z = this->particleList[j].position.z;
			this->particleList[i].rgb.x = this->particleList[j].rgb.x;
			this->particleList[i].rgb.y = this->particleList[j].rgb.y;
			this->particleList[i].rgb.z = this->particleList[j].rgb.z;
			this->particleList[i].velocity = this->particleList[j].velocity;
			this->particleList[i].active = this->particleList[j].active;
			i--;
			j--;
		}
		// Now insert it into the particle array in the correct depth order.
		this->particleList[index].position.x = position.x;
		this->particleList[index].position.y = position.y;
		this->particleList[index].position.z = position.z;
		this->particleList[index].rgb.x = rgb.x;
		this->particleList[index].rgb.y = rgb.y;
		this->particleList[index].rgb.z = rgb.z;
		this->particleList[index].velocity = velocity;
		this->particleList[index].active = true;
	}
}

void ParticleSystem::UpdateParticles(float frameTime) 
{
	// Each frame we update all the particles by making them move downwards using their position, velocity, and the frame time.
	for (int i = 0; i < this->currentParticleCount; i++)
	{
		this->particleList[i].position.y = this->particleList[i].position.y - (this->particleList[i].velocity * frameTime * 0.1f);
	}
}

void ParticleSystem::KillParticles() 
{

	for (int i = 0; i < this->maxParticles; i++)
	{
		if ((this->particleList[i].active == true) && (this->particleList[i].position.y < -10.0f)) //rather how deep from start pos will it go
		{
			this->particleList[i].active = false;
			this->currentParticleCount--;

			// Now shift all the live particles back up the array to erase the destroyed particle and keep the array sorted correctly.
			for (int j = i; j < this->maxParticles - 1; j++)
			{
				this->particleList[j].position.x = this->particleList[j + 1].position.x;
				this->particleList[j].position.y = this->particleList[j + 1].position.y;
				this->particleList[j].position.z = this->particleList[j + 1].position.z;
				this->particleList[j].rgb.x = this->particleList[j + 1].rgb.x;
				this->particleList[j].rgb.y = this->particleList[j + 1].rgb.y;
				this->particleList[j].rgb.z = this->particleList[j + 1].rgb.z;
				this->particleList[j].velocity = this->particleList[j + 1].velocity;
				this->particleList[j].active = this->particleList[j + 1].active;
			}
		}
	}
}

bool ParticleSystem::UpdateBuffers(ID3D11DeviceContext * deviceContext) //copied
{
	int index = 0;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Vertex3D* verticesPtr;
	// Initialize vertex array to zeros at first.
	//memset(this->vertices, 0, (sizeof(Vertex3D) * this->vertexCount));

	// Now build the vertex array from the particle list array.  Each particle is a quad made out of two triangles.

	for (int i = 0; i < this->currentParticleCount; i++)
	{
		DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(this->particleList[i].rgb.x, this->particleList[i].rgb.y, this->particleList[i].rgb.z, 1.0f);
		// Bottom left.
		this->body.at(index).x = this->particleList[i].position.x - this->particleSize;
		this->body.at(index).y = this->particleList[i].position.y - this->particleSize;
		this->body.at(index).z = this->particleList[i].position.z;
		this->body.at(index).u = 0.0f;
		this->body.at(index).v = 1.0f;
		index++;

		// Top left.
		this->body.at(index).x = this->particleList[i].position.x - this->particleSize;
		this->body.at(index).y = this->particleList[i].position.y + this->particleSize;
		this->body.at(index).z = this->particleList[i].position.z;
		this->body.at(index).u = 0.f;
		this->body.at(index).v = 0.f;
		index++;

		// Bottom right.
		this->body.at(index).x = this->particleList[i].position.x + this->particleSize;
		this->body.at(index).y = this->particleList[i].position.y - this->particleSize;
		this->body.at(index).z = this->particleList[i].position.z;
		this->body.at(index).u = 1.f;
		this->body.at(index).v = 1.f;
		index++;

		// Bottom right.
		this->body.at(index).x = this->particleList[i].position.x + this->particleSize;
		this->body.at(index).y = this->particleList[i].position.y - this->particleSize;
		this->body.at(index).z = this->particleList[i].position.z;
		this->body.at(index).u = 1.f;
		this->body.at(index).v = 1.f;
		index++;

		// Top left.
		this->body.at(index).x = this->particleList[i].position.x - this->particleSize;
		this->body.at(index).y = this->particleList[i].position.y + this->particleSize;
		this->body.at(index).z = this->particleList[i].position.z;
		this->body.at(index).u = 0.f;
		this->body.at(index).v = 0.f;
		index++;

		// Top right.
		this->body.at(index).x = this->particleList[i].position.x + this->particleSize;
		this->body.at(index).y = this->particleList[i].position.y + this->particleSize;
		this->body.at(index).z = this->particleList[i].position.z;
		this->body.at(index).u = 1.f;
		this->body.at(index).v = 0.f;
		index++;
	}

	// Lock the vertex buffer.
	result = deviceContext->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (Vertex3D*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)this->body.data(), (sizeof(Vertex3D) * this->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(this->vertexBuffer, 0);
	return true;
}

void ParticleSystem::calculateModelVectors()
{
	int faceCount = vertexCount / 3;
	int index = 0;
	NM_Vertex vertex1;
	NM_Vertex vertex2;
	NM_Vertex vertex3;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 binormal;
	DirectX::XMFLOAT3 normal;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (int i = 0; i < faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.x = body.at(index).x;
		vertex1.y = body.at(index).y;
		vertex1.z = body.at(index).z;
		vertex1.u = body.at(index).u;
		vertex1.v = body.at(index).v;
		vertex1.nx = body.at(index).nx;
		vertex1.ny = body.at(index).ny;
		vertex1.nz = body.at(index).nz;
		index++;

		vertex2.x = body.at(index).x;
		vertex2.y = body.at(index).y;
		vertex2.z = body.at(index).z;
		vertex2.u = body.at(index).u;
		vertex2.v = body.at(index).v;
		vertex2.nx = body.at(index).nx;
		vertex2.ny = body.at(index).ny;
		vertex2.nz = body.at(index).nz;
		index++;

		vertex3.x = body.at(index).x;
		vertex3.y = body.at(index).y;
		vertex3.z = body.at(index).z;
		vertex3.u = body.at(index).u;
		vertex3.v = body.at(index).v;
		vertex3.nx = body.at(index).nx;
		vertex3.ny = body.at(index).ny;
		vertex3.nz = body.at(index).nz;
		index++;
		// Calculate the tangent and binormal of that face.
		calculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal, normal);

		// Calculate the new normal using the tangent and binormal.
		calculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		body.at(index - 1).nx = normal.x;
		body.at(index - 1).ny = normal.y;
		body.at(index - 1).nz = normal.z;
		body.at(index - 1).tx = tangent.x;
		body.at(index - 1).ty = tangent.y;
		body.at(index - 1).tz = tangent.z;
		body.at(index - 1).bx = binormal.x;
		body.at(index - 1).by = binormal.y;
		body.at(index - 1).bz = binormal.z;

		body.at(index - 2).nx = normal.x;
		body.at(index - 2).ny = normal.y;
		body.at(index - 2).nz = normal.z;
		body.at(index - 2).tx = tangent.x;
		body.at(index - 2).ty = tangent.y;
		body.at(index - 2).tz = tangent.z;
		body.at(index - 2).bx = binormal.x;
		body.at(index - 2).by = binormal.y;
		body.at(index - 2).bz = binormal.z;

		body.at(index - 3).nx = normal.x;
		body.at(index - 3).ny = normal.y;
		body.at(index - 3).nz = normal.z;
		body.at(index - 3).tx = tangent.x;
		body.at(index - 3).ty = tangent.y;
		body.at(index - 3).tz = tangent.z;
		body.at(index - 3).bx = binormal.x;
		body.at(index - 3).by = binormal.y;
		body.at(index - 3).bz = binormal.z;
	}
}

void ParticleSystem::calculateTangentBinormal(NM_Vertex vertex1, NM_Vertex vertex2, NM_Vertex vertex3, DirectX::XMFLOAT3 & tangent, DirectX::XMFLOAT3 & binormal, DirectX::XMFLOAT3 & normal)
{
	// Calculate the two vectors for this face.
	float vector1[3];
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;
	float vector2[3];
	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	float tuVector[2];
	float tvVector[2];
	tuVector[0] = vertex2.u - vertex1.u;
	tvVector[0] = vertex2.v - vertex1.v;
	tuVector[1] = vertex3.u - vertex1.u;
	tvVector[1] = vertex3.v - vertex1.v;

	// Calculate the denominator of the tangent/binormal equation.
	float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	float length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}

void ParticleSystem::calculateNormal(DirectX::XMFLOAT3 tangent, DirectX::XMFLOAT3 binormal, DirectX::XMFLOAT3 & normal)
{
	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	float length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;
}
