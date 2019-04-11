#include "Model.h"
Model::Model()
{
	this->indexBuffer = IndexBuffer();
	this->SamplerState = nullptr;
	this->shader = nullptr;

}

Model::~Model()
{
}

void Model::setShader(Shader *& theShader)
{
	this->shader = theShader;
}

void Model::setTexture(std::string file)
{
	texture.setTexture(file);
}

void Model::setMesh(std::vector<Vertex3D> aMesh,DWORD* indices, int numberOfIndices)
{
	this->mesh = aMesh;
	this->vertexCount = aMesh.size();
	this->vertexBuffer.initialize(aMesh.data(), aMesh.size());
	this->indexBuffer.initialize(indices, numberOfIndices);
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
	System::getDeviceContext()->PSSetShaderResources(0, 1, &this->texture.getTexture());
	System::getDeviceContext()->PSSetShaderResources(1, 1, &this->normal.getTexture());
	System::getDeviceContext()->IASetVertexBuffers(0, 1, &*this->vertexBuffer.GetAddressOf(), &*vertexBuffer.getStridePtr(), &offset);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	System::getDeviceContext()->PSSetSamplers(0, 1, &this->SamplerState);

	this->shader->renderShader(this->vertexCount,indexBuffer.getBufferSize());

}
