#include"FullscreenQuad.h"

FullscreenQuad::FullscreenQuad()
{
	vertexBuffer = 0;
}

FullscreenQuad::~FullscreenQuad()
{
}


bool FullscreenQuad::Initialize(ID3D11Device* device, int windowWidth, int windowHeight)
{
	bool result;

	result = InitializeBuffers(device, windowWidth, windowHeight);
	if (!result)
	{
		return false;
	}

	return true;
}


void FullscreenQuad::Shutdown()
{
	ShutdownBuffers();
}


void FullscreenQuad::insertBuffers(ID3D11DeviceContext* deviceContext)
{
	insertBuffer(deviceContext);
}

bool FullscreenQuad::InitializeBuffers(ID3D11Device* device, int windowWidth, int windowHeight)
{
	VertexType* vertices;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	HRESULT result;
	vertexCount = 6;

	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	vertices[0].x = 1.0f;
	vertices[0].y = 1.0f;
	vertices[0].z = 0.0f; // Top left.
	vertices[0].u = 0.0f;//the positions can be set to anything because it will be set in the shaders
	vertices[0].v = 0.0f;

	vertices[1].x = 1.0f;
	vertices[1].y = 1.0f;
	vertices[1].z = 0.0f; // Bottom right.
	vertices[1].u = 1.0f;
	vertices[1].v = 1.0f;

	vertices[2].x = 1.0f;
	vertices[2].y = 1.0f;
	vertices[2].z = 0.0f; // Bottom left.
	vertices[2].u = 0.0f;
	vertices[2].v = 1.0f;

	vertices[3].x = 1.0f;
	vertices[3].y = 1.0f;
	vertices[3].z = 0.0f; // Top left.
	vertices[3].u = 0.0f;
	vertices[3].v = 0.0f;

	vertices[4].x = 1.0f;
	vertices[4].y = 1.0f;
	vertices[4].z = 0.0f; // Top right.
	vertices[4].u = 1.0f;
	vertices[4].v = 0.0f;

	vertices[5].x = 1.0f;
	vertices[5].y = 1.0f;
	vertices[5].z = 0.0f; // Bottom right.
	vertices[5].u = 1.0f;
	vertices[5].v = 1.0f;



	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = nullptr;

	return true;
}


void FullscreenQuad::ShutdownBuffers()
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}
}


void FullscreenQuad::insertBuffer(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}