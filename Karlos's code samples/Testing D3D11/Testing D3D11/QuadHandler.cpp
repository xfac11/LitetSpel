#include "QuadHandler.h"
void QuadHandler::expandQuads()
{
	this->CAP += 6;
	QuadVertex* *temp = new QuadVertex*[this->CAP];
	for (int i = 0; i < this->nrOfQuads; i++)
	{
		temp[i] = this->Quads[i];
	}
	delete[]this->Quads;
	this->Quads = temp;
}

void QuadHandler::expandVertex()
{
	this->CAPVertex += 12;
	Vertex3D* temp = new Vertex3D[this->CAPVertex];
	for (int i = 0; i < this->nrOfVertex; i++)
	{
		temp[i] = this->quadVertex[i];
	}
	delete[]this->quadVertex;
	this->quadVertex = temp;
}

QuadHandler::QuadHandler()
{
	this->nrOfVertex = 0;
	this->CAP = 6;
	this->CAPVertex = 12;
	this->nrOfQuads = 0;
	this->Quads = new QuadVertex*[CAP];
	for (int i = 0; i < CAP; i++)
	{
		this->Quads[i] = nullptr;
	}
	this->quadVertex = new Vertex3D[CAPVertex];
}

QuadHandler::~QuadHandler()
{
	for (int i = 0; i < CAP; i++)
	{
		delete this->Quads[i];
	}
	delete[] this->Quads;
	delete[] this->quadVertex;
}

bool QuadHandler::addQuad(DirectX::XMFLOAT3 pos, float width, float height, float depth, int face, int & vertexCount)
{
	bool result = false;
	if (nrOfQuads == CAP)
	{
		this->expandQuads();
	}
	if (this->nrOfVertex == this->CAPVertex)
	{
		this->expandVertex();
	}
	result = true;
	Quads[nrOfQuads] = new QuadVertex(pos, width, height, depth, face);
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			Vertex3D temp = Quads[nrOfQuads]->getTri(j).getPoint(i);
			this->quadVertex[nrOfVertex] = temp;
			nrOfVertex++;
		}
	}
	nrOfQuads++;
	return result;
}

bool QuadHandler::addTri(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3, int & vertexCount)
{
	bool result = true;
	Vertex3D temp;
	Vertex3D temp2;
	Vertex3D temp3;
	temp.x = p1.x;
	temp.y = p1.y;
	temp.z = p1.z;
	temp.u = 0.0f;
	temp.v = 1.0f;
	temp2.x = p2.x;
	temp2.y = p2.y;
	temp2.z = p2.z;
	temp2.u = 0.5f;
	temp2.v = 0.5f;
	temp3.x = p3.x;
	temp3.y = p3.y;
	temp3.z = p3.z;
	temp3.u = 1.0f;
	temp3.v = 1.0f;
	this->quadVertex[nrOfVertex] = temp;
	nrOfVertex++;
	this->quadVertex[nrOfVertex] = temp2;
	nrOfVertex++;
	this->quadVertex[nrOfVertex] = temp3;
	nrOfVertex++;
	vertexCount += 3;
	return result;

}

//bool QuadHandler::createVertexBuffer(ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer, int & vertexCount)
//{
//	D3D11_SUBRESOURCE_DATA data;
//	HRESULT hr;
//	D3D11_BUFFER_DESC bufferDesc;
//	memset(&bufferDesc, 0, sizeof(bufferDesc));
//	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	bufferDesc.ByteWidth = this->nrOfVertex * sizeof(Vertex3D);
//	data.pSysMem = this->quadVertex;
//	hr = gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
//	if (FAILED(hr))
//	{
//		return false;
//	}
//	return true;
//}

//UINT32 QuadHandler::getSize(int id)
//{
//	return Quads[id]->getSize();
//}

QuadVertex *& QuadHandler::getQuad(int id)
{
	return  Quads[id];
}