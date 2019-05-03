#include "Primitives.h"
#include "System.h"



bool Primitives::CreateBox(btVector3 size)
{
	std::vector<VertexObject> vertices;
	std::vector<DWORD> indecesCounter;

	vertices.resize(8);
	indecesCounter.resize(24);
	{
		//vertices[0] = { -size.getX(), -size.getY(), -size.getZ() ,
		//			 1.0f, 0.0f, 0.0f };
		//vertices[1] = { -size.getX(),  size.getY(), -size.getZ() ,
		//				 1.0f, 0.0f, 0.0f };
		//vertices[2] = { size.getX(),  size.getY(), -size.getZ() ,
		//				 1.0f, 0.0f, 0.0f };
		//vertices[3] = { size.getX(), -size.getY(), -size.getZ() ,
		//				 1.0f, 0.0f, 0.0f };
		//vertices[4] = { -size.getX(), -size.getY(), size.getZ(),
		//				 0.0f, 0.0f, 1.0f };
		//vertices[5] = { -size.getX(),  size.getY(), size.getZ() ,
		//				 0.0f, 0.0f, 1.0f };
		//vertices[6] = { size.getX(),  size.getY(), size.getZ() ,
		//				 0.0f, 0.0f, 1.0f };
		//vertices[7] = { size.getX(), -size.getY(), size.getZ() ,
		//				 0.0f, 0.0f, 1.0f };

		vertices[0] = { -size.getX(), 0, -size.getZ() ,
					 1.0f, 0.0f, 0.0f };
		vertices[1] = { -size.getX(),2 * size.getY(), -size.getZ() ,
						 1.0f, 0.0f, 0.0f };
		vertices[2] = { size.getX(),2 * size.getY(), -size.getZ() ,
						 1.0f, 0.0f, 0.0f };
		vertices[3] = { size.getX(),0, -size.getZ() ,
						 1.0f, 0.0f, 0.0f };

		vertices[4] = { -size.getX(), 0, size.getZ(),
						 0.0f, 0.0f, 1.0f };
		vertices[5] = { -size.getX(),2 * size.getY(), size.getZ() ,
						 0.0f, 0.0f, 1.0f };
		vertices[6] = { size.getX(), 2 * size.getY(), size.getZ() ,
						 0.0f, 0.0f, 1.0f };
		vertices[7] = { size.getX(),0, size.getZ() ,
						 0.0f, 0.0f, 1.0f };



		indecesCounter[0] = 0;
		indecesCounter[1] = 1;
		indecesCounter[2] = 1;
		indecesCounter[3] = 2;
		indecesCounter[4] = 2;
		indecesCounter[5] = 3;
		indecesCounter[6] = 3;
		indecesCounter[7] = 0;

		indecesCounter[8] = 0;
		indecesCounter[9] = 4;
		indecesCounter[10] = 1;
		indecesCounter[11] = 5;
		indecesCounter[12] = 2;
		indecesCounter[13] = 6;
		indecesCounter[14] = 3;
		indecesCounter[15] = 7;

		indecesCounter[16] = 4;
		indecesCounter[17] = 5;
		indecesCounter[18] = 5;
		indecesCounter[19] = 6;
		indecesCounter[20] = 6;
		indecesCounter[21] = 7;
		indecesCounter[22] = 7;
		indecesCounter[23] = 4;
	}
	indices = 24;
	this->vertexBuffer.initialize(vertices.data(), 8, System::getDevice());
	this->indicesbuffer.initialize(indecesCounter.data(), indices, System::getDevice());
	return true;
}

Primitives::Primitives()
{
	this->indices = 0;
	this->world = nullptr;
}

void Primitives::Initialize(int shapes, btVector3 size)
{
	switch (shapes)
	{
	case 1:
		this->CreateBox(size);
		break;
	}
}

Primitives::~Primitives()
{
}

void Primitives::Draw(SimpleShader* shader)
{
	shader->setWorld(*this->world);
	const UINT offset = 0;
	const UINT stride = sizeof(VertexObject);
	// Set the vertex buffer
	System::getDeviceContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	System::getDeviceContext()->IASetIndexBuffer(indicesbuffer.getBuffer(), DXGI_FORMAT_R32_UINT, 0);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	System::getDeviceContext()->DrawIndexed(this->indices, 0, 0);
}
