//#include "ParticleHandler.h"
//
//bool ParticleHandler::InitializeParticleSystem()
//{
//	// Set the random deviation of where the particles can be located when emitted.
//	this->particleDeviation.x = 20.5f;
//	this->particleDeviation.y = 3.f;
//	this->particleDeviation.z = 20.5f;
//
//	// Set the speed and speed variation of particles.
//	this->particleVelocity = 10.f;
//	this->particleVelocityVariation = 2.f;
//
//	this->particleSize = 0.5f;
//	this->particlesPerSecond = 10.0f;
//	this->maxParticles = 5000;
//	this->particles = new Particle[this->maxParticles];
//	for (int i = 0; i < this->maxParticles; i++)// Initialize the particle list.
//	{
//		this->particles[i].getParticle().active = false;
//	}
//	this->currentParticleCount = 0;// Initialize the current particle count to zero since none are emitted yet.
//
//	// Clear the initial accumulated time for the particle per second emission rate.
//	this->accumulatedTime = 0.0f;
//	return true;
//}
//
//bool ParticleHandler::InitializeBuffers(ID3D11Device * device)
//{
//	D3D11_BUFFER_DESC vertexBufferDesc;
//	D3D11_BUFFER_DESC indexBufferDesc;
//	D3D11_SUBRESOURCE_DATA vertexData;
//	D3D11_SUBRESOURCE_DATA indexData;
//	HRESULT result;
//
//	// Set the maximum number of vertices in the vertex array.
//	this->vertexCount = //this->maxParticles * 6;
//
//	// Set the maximum number of indices in the index array.
//	this->indexCount = 6;//this->vertexCount;
//
//	// Create the vertex array for the particles that will be rendered.
//	this->vertices = new Vertex3D[this->vertexCount];
//
//	// Create the index array.
//	unsigned long* indices = new unsigned long[this->indexCount];
//
//	// Initialize vertex array to zeros at first.
//	memset(this->vertices, 0, (sizeof(Vertex3D) * this->vertexCount));
//
//	// Initialize the index array.
//	for (int i = 0; i < this->indexCount; i++)
//	{
//		indices[i] = i;
//	}
//
//	// Set up the description of the dynamic vertex buffer.
//	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	vertexBufferDesc.ByteWidth = sizeof(Vertex3D) * this->vertexCount;
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	vertexBufferDesc.MiscFlags = 0;
//	vertexBufferDesc.StructureByteStride = 0;
//
//	// Give the subresource structure a pointer to the vertex data.
//	vertexData.pSysMem = this->vertices;
//	vertexData.SysMemPitch = 0;
//	vertexData.SysMemSlicePitch = 0;
//
//	// Now finally create the vertex buffer.
//	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &this->vertexBuffer);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Set up the description of the static index buffer.
//	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	indexBufferDesc.ByteWidth = sizeof(unsigned long) * this->indexCount;
//	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	indexBufferDesc.CPUAccessFlags = 0;
//	indexBufferDesc.MiscFlags = 0;
//	indexBufferDesc.StructureByteStride = 0;
//
//	// Give the subresource structure a pointer to the index data.
//	indexData.pSysMem = indices;
//	indexData.SysMemPitch = 0;
//	indexData.SysMemSlicePitch = 0;
//
//	// Create the index buffer.
//	result = device->CreateBuffer(&indexBufferDesc, &indexData, &this->indexBuffer);
//	if (FAILED(result))
//	{
//		return false;
//	}
//	delete[] indices;
//	return true;
//}
//
//void ParticleHandler::EmitParticles(float frameTime)
//{
//	bool emitParticle = false;
//	bool found = false;;
//	DirectX::XMFLOAT3 position;
//	float velocity;
//	DirectX::XMFLOAT3 rgb; //not using rgb in vertex 3D
//	int index=0;
//	this->accumulatedTime += frameTime;// Increment the frame time.
//
//	// Check if it is time to emit a new particle or not.
//	if (this->accumulatedTime > (1.0f / this->particlesPerSecond))
//	{
//		this->accumulatedTime = 0.0f;
//		emitParticle = true;
//	}
//
//	// If there are particles to emit then emit one per frame.
//	if ((emitParticle == true) && (this->currentParticleCount < (this->maxParticles - 1)))
//	{
//		this->currentParticleCount++;
//
//		// Now generate the randomized particle properties.
//		position.x = (((float)rand() - (float)rand()) / RAND_MAX) * this->particleDeviation.x;
//		position.y = (((float)rand() - (float)rand()) / RAND_MAX) * this->particleDeviation.y;
//		position.z = (((float)rand() - (float)rand()) / RAND_MAX) * this->particleDeviation.z;
//
//		velocity = this->particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * this->particleVelocityVariation;
//
//		rgb.x = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
//		rgb.y = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
//		rgb.z = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
//
//		// Now since the particles need to be rendered from back to front for blending we have to sort the particle array.
//		// We will sort using Z depth so we need to find where in the list the particle should be inserted.
//		index = 0;
//		found = false;
//		while (!found)
//		{
//			if ((this->particles[index].getParticle().active == false) || (this->particles[index].getParticle().position.z < position.z))
//				found = true;
//			else
//				index++;
//		}
//
//		// Now that we know the location to insert into we need to copy the array over by one position from the index to make room for the new particle.
//		int i = this->currentParticleCount;
//		int j = i - 1;
//
//		while (i != index)
//		{
//			this->particles[i].setPosition(this->particles[j].getPosition());
//			//this->particles[i].getParticle().position.x = this->particles[j].getParticle().position.x;
//			//this->particles[i].getParticle().position.y = this->particles[j].getParticle().position.y;
//			//this->particles[i].getParticle().position.z = this->particles[j].getParticle().position.z;
//			this->particles[i].getParticle().rgb.x = this->particles[j].getParticle().rgb.x;
//			this->particles[i].getParticle().rgb.y = this->particles[j].getParticle().rgb.y;
//			this->particles[i].getParticle().rgb.z = this->particles[j].getParticle().rgb.z;
//			this->particles[i].getParticle().velocity = this->particles[j].getParticle().velocity;
//			this->particles[i].getParticle().active = this->particles[j].getParticle().active;
//			i--;
//			j--;
//		}
//		// Now insert it into the particle array in the correct depth order.
//		this->particles[index].setPosition(position);
//		
//		//this->particles[index].getParticle().position.x = position.x;
//		//this->particles[index].getParticle().position.y = position.y;
//		//this->particles[index].getParticle().position.z = position.z;
//		this->particles[index].getParticle().rgb.x = rgb.x;
//		this->particles[index].getParticle().rgb.y = rgb.y;
//		this->particles[index].getParticle().rgb.z = rgb.z;
//		this->particles[index].getParticle().velocity = velocity;
//		this->particles[index].getParticle().active = true;
//	}
//}
//
//void ParticleHandler::UpdateParticles(float frameTime)
//{
//	for (int i = 0; i < this->currentParticleCount; i++)
//	{
//		//this->particles[i].getParticle().position.y = this->particles[i].getParticle().position.y - (this->particles[i].getParticle().velocity * frameTime * 0.1f);
//		this->particles[i].setY(this->particles[i].getPosition().y - (this->particles[i].getParticle().velocity* frameTime*0.1f));
//	}
//}
//
//void ParticleHandler::KillParticles()
//{
//	// Kill all the particles that have gone below a certain height range.
//	for (int i = 0; i < this->maxParticles; i++)
//	{
//		if ((this->particles[i].getParticle().active == true) && (this->particles[i].getPosition().y < -10.0f)) //rather how deep from start pos will it go
//		{
//			this->particles[i].getParticle().active = false;
//			this->currentParticleCount--;
//
//			// Now shift all the live particles back up the array to erase the destroyed particle and keep the array sorted correctly.
//			for (int j = i; j < this->maxParticles - 1; j++)
//			{
//				this->particles[j].setPosition(this->particles[j + 1].getPosition());
//				//this->particles[j].getParticle().position.x = this->particles[j + 1].getParticle().position.x;
//				//this->particles[j].getParticle().position.y = this->particles[j + 1].getParticle().position.y;
//				//this->particles[j].getParticle().position.z = this->particles[j + 1].getParticle().position.z;
//				this->particles[j].getParticle().rgb.x = this->particles[j + 1].getParticle().rgb.x;
//				this->particles[j].getParticle().rgb.y = this->particles[j + 1].getParticle().rgb.y;
//				this->particles[j].getParticle().rgb.z = this->particles[j + 1].getParticle().rgb.z;
//				this->particles[j].getParticle().velocity = this->particles[j + 1].getParticle().velocity;
//				this->particles[j].getParticle().active = this->particles[j + 1].getParticle().active;
//			}
//		}
//	}
//}
//
//bool ParticleHandler::LoadTexture(ID3D11Device *& device, ID3D11DeviceContext *& deviceContext, std::string filename)
//{
//	theTexture.setTexture(device, deviceContext, filename);
//	return true;
//}
//
//bool ParticleHandler::UpdateBuffers(ID3D11DeviceContext * deviceContext)
//{
//	int index = 0;
//	HRESULT result;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	Vertex3D* verticesPtr;
//	// Initialize vertex array to zeros at first.
//	memset(this->vertices, 0, (sizeof(Vertex3D) * this->vertexCount));
//	// Now build the vertex array from the particle list array.  Each particle is a quad made out of two triangles.
//
//	for (int i = 0; i < this->currentParticleCount; i++)
//	{
//		DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(this->particles[i].getParticle().rgb.x, this->particles[i].getParticle().rgb.y, this->particles[i].getParticle().rgb.z, 1.0f);
//		// Bottom left.
//		//this->vertices[index].x = this->particles[i].getParticle().position.x - this->particleSize;
//		//this->vertices[index].y = this->particles[i].getParticle().position.y - this->particleSize;
//		//this->vertices[index].z = this->particles[i].getParticle().position.z;
//		this->vertices[index].x = this->particles[i].getPosition().x - this->particleSize;
//		this->vertices[index].y = this->particles[i].getPosition().y - this->particleSize;
//		this->vertices[index].z = this->particles[i].getPosition().z;
//		this->vertices[index].u = 0.0f;
//		this->vertices[index].v = 1.0f;
//		index++;
//
//		// Top left.
//		//this->vertices[index].x = this->particles[i].getParticle().position.x - this->particleSize;
//		//this->vertices[index].y = this->particles[i].getParticle().position.y + this->particleSize;
//		//this->vertices[index].z = this->particles[i].getParticle().position.z;
//		this->vertices[index].x = this->particles[i].getPosition().x - this->particleSize;
//		this->vertices[index].y = this->particles[i].getPosition().y + this->particleSize;
//		this->vertices[index].z = this->particles[i].getPosition().z;
//		this->vertices[index].u = 0.f;
//		this->vertices[index].v = 0.f;
//		index++;
//
//		// Bottom right.
//		//this->vertices[index].x = this->particles[i].getParticle().position.x + this->particleSize;
//		//this->vertices[index].y = this->particles[i].getParticle().position.y - this->particleSize;
//		//this->vertices[index].z = this->particles[i].getParticle().position.z;
//		this->vertices[index].x = this->particles[i].getPosition().x + this->particleSize;
//		this->vertices[index].y = this->particles[i].getPosition().y - this->particleSize;
//		this->vertices[index].z = this->particles[i].getPosition().z;
//		this->vertices[index].u = 1.f;
//		this->vertices[index].v = 1.f;
//		index++;
//
//		// Bottom right.
//		//this->vertices[index].x = this->particles[i].getParticle().position.x + this->particleSize;
//		//this->vertices[index].y = this->particles[i].getParticle().position.y - this->particleSize;
//		//this->vertices[index].z = this->particles[i].getParticle().position.z;
//		this->vertices[index].x = this->particles[i].getPosition().x + this->particleSize;
//		this->vertices[index].y = this->particles[i].getPosition().y - this->particleSize;
//		this->vertices[index].z = this->particles[i].getPosition().z;
//		this->vertices[index].u = 1.f;
//		this->vertices[index].v = 1.f;
//		index++;
//
//		// Top left.
//		//this->vertices[index].x = this->particles[i].getParticle().position.x - this->particleSize;
//		//this->vertices[index].y = this->particles[i].getParticle().position.y + this->particleSize;
//		//this->vertices[index].z = this->particles[i].getParticle().position.z;
//		this->vertices[index].x = this->particles[i].getPosition().x - this->particleSize;
//		this->vertices[index].y = this->particles[i].getPosition().y + this->particleSize;
//		this->vertices[index].z = this->particles[i].getPosition().z;
//		this->vertices[index].u = 0.f;
//		this->vertices[index].v = 0.f;
//		index++;
//
//		// Top right.
//		//this->vertices[index].x = this->particles[i].getParticle().position.x + this->particleSize;
//		//this->vertices[index].y = this->particles[i].getParticle().position.y + this->particleSize;
//		//this->vertices[index].z = this->particles[i].getParticle().position.z;
//		this->vertices[index].x = this->particles[i].getPosition().x + this->particleSize;
//		this->vertices[index].y = this->particles[i].getPosition().y + this->particleSize;
//		this->vertices[index].z = this->particles[i].getPosition().z;
//		this->vertices[index].u = 1.f;
//		this->vertices[index].v = 0.f;
//		index++;
//	}
//
//	// Lock the vertex buffer.
//	result = deviceContext->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Get a pointer to the data in the vertex buffer.
//	verticesPtr = (Vertex3D*)mappedResource.pData;
//
//	// Copy the data into the vertex buffer.
//	memcpy(verticesPtr, (void*)this->vertices, (sizeof(Vertex3D) * this->vertexCount));
//
//	// Unlock the vertex buffer.
//	deviceContext->Unmap(this->vertexBuffer, 0);
//	return true;
//}
//
//ParticleHandler::ParticleHandler()
//{
//	this->particles = nullptr;
//	this->vertices = nullptr;
//	this->vertexBuffer = nullptr;
//	this->indexBuffer = nullptr;
//	this->SamplerState = nullptr;
//}
//
//ParticleHandler::~ParticleHandler()
//{
//}
//
//bool ParticleHandler::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::string textureName)
//{
//	// Load the texture that is used for the particles.
//	bool result = LoadTexture(device, deviceContext, textureName);
//	if (!result)
//	{
//		return false;
//	}
//
//	// Initialize the particle system.
//	result = InitializeParticleSystem();
//	if (!result)
//	{
//		return false;
//	}
//
//	// Create the buffers that will be used to render the particles with.
//	result = InitializeBuffers(device);
//	if (!result)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//void ParticleHandler::Shutdown()
//{
//	if (this->particles)
//	{
//		delete[] particles;
//		this->particles = nullptr;
//	}
//	if(this->vertices)
//	{
//		delete[] vertices;
//		this->vertices = nullptr;
//	}
//	if (this->indexBuffer)
//	{
//		this->indexBuffer->Release();
//		this->indexBuffer = nullptr;
//	}
//	if (this->vertexBuffer)
//	{
//		this->vertexBuffer->Release();
//		this->vertexBuffer = nullptr;
//	}
//	if (this->SamplerState)
//	{
//		this->SamplerState->Release();
//		this->SamplerState = nullptr;
//	}
//}
//
//bool ParticleHandler::Frame(float frameTime, ID3D11DeviceContext * deviceContext)
//{
//	bool result;
//	KillParticles();
//	EmitParticles(frameTime);
//	UpdateParticles(frameTime);
//	result = UpdateBuffers(deviceContext);
//	if (!result)
//	{
//		return false;
//	}
//	return true;
//}
//
//void ParticleHandler::Render(ColorShader & shader, ID3D11DeviceContext * deviceContext)
//{
//	// Set vertex buffer stride and offset.
//	unsigned int stride = sizeof(Vertex3D);
//	unsigned int offset = 0;
//	deviceContext->PSSetShaderResources(0, 1, &this->theTexture.getTexture());
//
//	// Set the vertex buffer to active in the input assembler so it can be rendered.
//	deviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
//
//	// Set the index buffer to active in the input assembler so it can be rendered.
//	deviceContext->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
//
//	// Set the type of primitive that should be rendered from this vertex buffer.
//	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	deviceContext->PSSetSamplers(0, 1, &this->SamplerState);
//	shader.RenderShader(deviceContext, indexCount);
//}
//
//void ParticleHandler::setWorld()
//{
//	for (int i = 0; i < maxParticles; i++)
//	{
//		this->particles[i].setWorld();
//	}
//}
//
//DirectX::XMFLOAT4X4 ParticleHandler::getWorld(int id)
//{
//	return particles[id].getWorld();
//}
//
//int ParticleHandler::getNrOfParticles()
//{
//	return currentParticleCount;
//}
//
//void ParticleHandler::billboard(DirectX::XMFLOAT3 camPos)
//{
//	for(int i =0;i< currentParticleCount;i++)
//	{ 
//		this->particles[i].billboard(camPos);
//	}
//}
//
//void ParticleHandler::setSampler(ID3D11Device *& gDevice)
//{
//	D3D11_SAMPLER_DESC desc;
//	ZeroMemory(&desc, sizeof(desc));
//	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	desc.MinLOD = 0;
//	desc.MaxLOD = D3D11_FLOAT32_MAX;
//	HRESULT hr = gDevice->CreateSamplerState(&desc, &this->SamplerState);
//	if (FAILED(hr))
//	{
//		//		MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", "error", MB_OK);
//	}
//}
