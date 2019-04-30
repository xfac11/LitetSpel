#include "Primitives.h"
#include "System.h"



bool Primitives::CreateQuad()
{
	vertices.resize(8);
	indices.resize(24);

	vertices[0] = { -1.0f, -1.0f, 0.0f , 
					 1.0f, 0.0f, 0.0f };
	vertices[1] = { -1.0f,  1.0f, 0.0f ,
					 1.0f, 0.0f, 0.0f  };
	vertices[2] = {  1.0f,  1.0f, 0.0f ,
					 1.0f, 0.0f, 0.0f };
	vertices[3] = {  1.0f, -1.0f, 0.0f ,
					 1.0f, 0.0f, 0.0f };
	
	vertices[4] = { -1.0f, -1.0f, 1.0f ,
				     0.0f, 0.0f, 1.0f };
	vertices[5] = { -1.0f,  1.0f, 1.0f ,
					 0.0f, 0.0f, 1.0f };
	vertices[6] = { 1.0f,  1.0f, 1.0f ,
					 0.0f, 0.0f, 1.0f };
	vertices[7] = { 1.0f, -1.0f, 1.0f ,
					 0.0f, 0.0f, 1.0f };

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 1;
	indices[3] = 2;
	indices[4] = 2;
	indices[5] = 3;
	indices[6] = 3;
	indices[7] = 0;

	indices[8] = 0;
	indices[9] = 4;
	indices[10] = 1;
	indices[11] = 5;
	indices[12] = 2;
	indices[13] = 6;
	indices[14] = 3;
	indices[15] = 7;

	indices[16] = 4;
	indices[17] = 5;
	indices[18] = 5;
	indices[19] = 6;
	indices[20] = 6;
	indices[21] = 7;
	indices[22] = 7;
	indices[23] = 4;

	indice = 24;
	this->vertexBuffer.initialize(vertices.data(), 8, System::getDevice());
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
	shader->setWorld(*this->world);
	const UINT offset = 0;
	const UINT stride = sizeof(VertexObject);
	// Set the vertex buffer
	System::getDeviceContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	System::getDeviceContext()->IASetIndexBuffer(indicesbuffer.getBuffer(), DXGI_FORMAT_R32_UINT, 0);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	System::getDeviceContext()->DrawIndexed(this->indice, 0, 0);


}
