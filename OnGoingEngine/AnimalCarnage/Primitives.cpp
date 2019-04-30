#include "Primitives.h"
#include "System.h"



bool Primitives::CreateQuad()
{
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

	indice = 6;
	this->vertexBuffer.initialize(vertices.data(), 4, System::getDevice());
	this->indicesbuffer.initialize(indices.data(), indice, System::getDevice());
	return true;
}

Primitives::Primitives()
{
}

void Primitives::initialize(int shapes)
{
	switch (shapes)
	{
	case 1:
		this->CreateQuad();
		break;
	}
}

Primitives::~Primitives()
{
}

void Primitives::draw(SimpleShader* shader)
{
	shader->setWorld(this->getWorld());
	const UINT offset = 0;
	const UINT stride = sizeof(VertexObject);
	// Set the vertex buffer
	System::getDeviceContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	System::getDeviceContext()->IASetIndexBuffer(indicesbuffer.getBuffer(), DXGI_FORMAT_R32_UINT, 0);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	System::getDeviceContext()->DrawIndexed(this->indice, 0, 0);


}
