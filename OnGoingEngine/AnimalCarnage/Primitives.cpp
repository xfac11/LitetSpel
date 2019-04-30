#include "Primitives.h"


bool Primitives::CreateQuad()
{
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;

	vertices.resize(4);
	indices.resize(6);

	vertices[0] = { -1.0f, -1.0f, 0.0f , 
					 0.0f, 0.0f, 1.0f };
	vertices[1] = { -1.0f,  1.0f, 0.0f ,
					 0.0f, 0.0f, 1.0f  };
	vertices[2] = {  1.0f,  1.0f, 0.0f ,
					 0.0f, 0.0f, 1.0f };
	vertices[3] = {  1.0f, -1.0f, 0.0f ,
					 0.0f, 0.0f, 1.0f };

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	D3D11_BUFFER_DESC vertexBufferDesc{ 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	D3D11_BUFFER_DESC indicesBufferDesc{ 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, 0 };
	D3D11_SUBRESOURCE_DATA indexSubresourceData{};
	indice = 6;

	vertexBufferDesc.ByteWidth = (sizeof(Vertex)) * 4;
	vertexSubresourceData.pSysMem = vertices.data();
	HRESULT hr =	System::getDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &VerticesBuffer);
	 if (FAILED(hr))
	 {
		 MessageBox(NULL, "Create VertexBuffer failed.",
			 "Didn't VertexBuffer for QUAD", MB_OK);
		 return false;
	 }
	indicesBufferDesc.ByteWidth = sizeof(UINT) * 6;
	indexSubresourceData.pSysMem = indices.data();
	hr = System::getDevice()->CreateBuffer(&indicesBufferDesc, &indexSubresourceData, &IndicesBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Create VertexBuffer failed.",
			"Didn't VertexBuffer for QUAD", MB_OK);
		return false;
	}
	return true;
}

Primitives::Primitives(SHAPES shapes)
{
	switch (shapes)
	{
	case QUAD:
		this->CreateQuad();
		break;
	}
}

Primitives::~Primitives()
{
}
