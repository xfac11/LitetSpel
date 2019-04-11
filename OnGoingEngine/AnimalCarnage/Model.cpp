#include "Model.h"
#include"System.h"
Model::Model()
{
	this->indexBuffer = IndexBuffer();
	this->SamplerState = nullptr;
	this->theShader = nullptr;
	this->texture = new Texture;
	this->normalMap = new Texture;

}

Model::~Model()
{
	if (this->SamplerState != nullptr)
		this->SamplerState->Release();
	if (this->texture != nullptr)
	{
		this->texture->cleanUp();
		delete this->texture;
	}
	if (this->normalMap != nullptr)
	{
		this->normalMap->cleanUp();
		delete this->normalMap;
	}
	//if(this->texture)
	//	this->texture.cleanUp();
}

void Model::setShader(Shader *theShader)
{
	this->theShader = theShader;
}

Shader * Model::getShader()
{
	return this->theShader;
}

void Model::setTexture(std::string file)
{
	texture->setTexture(file);
}

void Model::setMesh(std::vector<Vertex3D> aMesh,DWORD* indices, int numberOfIndices)
{
	this->mesh = aMesh;
	this->vertexCount = int(aMesh.size());
	this->vertexBuffer.initialize(aMesh.data(), UINT(aMesh.size()),System::getDevice());
	this->indexBuffer.initialize(indices, numberOfIndices,System::getDevice());
}

void Model::setSampler()
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
	HRESULT hr = System::getDevice()->CreateSamplerState(&desc, &this->SamplerState);
	if (FAILED(hr))
	{
		//MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", "error", MB_OK);
		//deal with error. Log it maybe

	}
}
void Model::setSampler(D3D11_TEXTURE_ADDRESS_MODE type, D3D11_FILTER filter,D3D11_COMPARISON_FUNC comFunc)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = filter;
	desc.AddressU = type;
	desc.AddressV = type;
	desc.AddressW = type;
	desc.ComparisonFunc = comFunc;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = System::getDevice()->CreateSamplerState(&desc, &this->SamplerState);
	if (FAILED(hr))
	{
		//MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", "error", MB_OK);
		//deal with error. Log it maybe

	}
}

void Model::draw()
{
	UINT32 offset = 0;
	System::getDeviceContext()->PSSetShaderResources(0, 1, &this->texture->getTexture());
	if (this->normalMap != nullptr)
	{
		System::getDeviceContext()->PSSetShaderResources(1, 1, &this->normalMap->getTexture());
	}
	System::getDeviceContext()->IASetVertexBuffers(0, 1, &*this->vertexBuffer.GetAddressOf(), &*vertexBuffer.getStridePtr(), &offset);
	UINT stride = sizeof(VERTEX);
//	UINT offset = 0;
	//devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	System::getDeviceContext()->IASetIndexBuffer(indexBuffer.getBuffer(), DXGI_FORMAT_R32_UINT, offset);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	System::getDeviceContext()->PSSetSamplers(0, 1, &this->SamplerState);

	this->theShader->renderShader(this->vertexCount,indexBuffer.getBufferSize());

}
