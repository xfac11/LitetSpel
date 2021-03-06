#include "Model.h"
#include"System.h"
Model::Model()
{
	this->gotSkeleton = false;
	this->indexBuffer = IndexBuffer();
	this->SamplerState = nullptr;
	this->theShader = nullptr;
	shared_ptr<Texture> t;
	texture = t;
	this->normalMap = new Texture;
	shared_ptr<Texture> glowtex;
	glowMap = glowtex;
	this->type = Opaque;
	this->hasMask = false;
	this->hasGlowMap = false;
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
	this->repeatXY = DirectX::XMFLOAT4(1, 1, 1, 1);
	this->colorMask = DirectX::XMFLOAT4(1, 1, 1, 1);

}

//void Model::operator=(const Model & obj)
//{
//
//	//this->texture
//	//this->glowMap
//
//	this->gotSkeleton = obj.gotSkeleton;
//	this->colorMask = obj.colorMask;
//	this->repeatXY = obj.repeatXY;
//	this->mesh.data = obj.mesh.data;
//	this->vertexBuffer = obj.vertexBuffer;
//
//	//material
//	//mesh
//	//texture set( matdiff, matdiff)
//	//texture assetmanage->getText
//	//m->setTexture
//
//	//glowmap
//
//	//skeleton
//	//
//	//joint
//	//weights
//	//animation
//	//keyframesPack
//
//
//	//
//}

Model::~Model()
{
	if (this->SamplerState != nullptr)
		this->SamplerState->Release();
	if (texture != nullptr)
	{
		 texture  = nullptr;
	}
	if (this->normalMap != nullptr)
	{
		delete this->normalMap;
	}
	if (this->glowMap != nullptr)
	{
		glowMap = nullptr;
	}
	/*if (this->mask != nullptr)
	{*/
		this->mask = nullptr;
	//}

}

void Model::setShader(Shader *theShader)
{
	this->theShader = theShader;
}

void Model::setRepeat(float x, float y)
{
	this->repeatXY = DirectX::XMFLOAT4(x, y, 1, 1);
}

void Model::setMaskColor(DirectX::XMFLOAT4 colorMask)
{
	this->colorMask = colorMask;
}

DirectX::XMFLOAT4 & Model::getMaskColor()
{
	return this->colorMask;
}

bool Model::hasMaskColor()
{
	return this->hasMask;
}

DirectX::XMFLOAT4 & Model::getRepeat()
{
	return this->repeatXY;
}

int Model::getOpacity()
{
	return this->type;
}

void Model::setOpacity(Opacity type)
{
	this->type = type;
}

void Model::setGotSkeleton(bool gotSkltn)
{
	this->gotSkeleton = gotSkltn;
}


Shader * Model::getShader()
{
	return this->theShader;
}

void Model::setTexture(std::string file, int mipLevels)
{
	//texture
	texture->setTexture(file, mipLevels);
	if (texture->isTransparent())
	{
		this->type = Transparent;
	}
	else
	{
		this->type = Opaque;
	}
}

void Model::setGlowMap(shared_ptr<Texture> t)
{
	glowMap = t;
	this->hasGlowMap = true;
}

void Model::setGlowMap(std::string file)
{
	this->glowMap->setTexture(file,1);
	this->hasGlowMap = true;
}

void Model::setMask(std::string file)
{
	this->mask->setTexture(file, -1);
	this->hasMask = true;
}


void Model::setMesh(std::vector<Vertex3D> &aMesh)// , DWORD* indices, int numberOfIndices)
{
	this->mesh = aMesh;
	this->vertexCount = int(aMesh.size());
	this->vertexBuffer.initialize(aMesh.data(), UINT(aMesh.size()),System::getDevice());
	//this->indexBuffer.initialize(indices, numberOfIndices,System::getDevice());
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

void Model::drawOnlyVertex()
{
	UINT32 offset = 0;
	/*System::getDeviceContext()->PSSetShaderResources(0, 1, &this->texture->getTexture());
	if (this->normalMap != nullptr)
	{
		System::getDeviceContext()->PSSetShaderResources(1, 1, &this->normalMap->getTexture());
	}*/
	//System::shaderManager->getDefShader()->setSkeleton(this->gotSkeleton);

	System::getDeviceContext()->IASetVertexBuffers(0, 1, &*this->vertexBuffer.GetAddressOf(), &*vertexBuffer.getStridePtr(), &offset);
	//	UINT offset = 0;
		//devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

		//System::getDeviceContext()->IASetIndexBuffer(indexBuffer.getBuffer(), DXGI_FORMAT_R32_UINT, offset);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//System::getDeviceContext()->PSSetSamplers(0, 1, &this->SamplerState);

	System::getDeviceContext()->Draw(mesh.size(), 0);
}

void Model::draw()
{
	UINT32 offset = 0;

	System::shaderManager->getDefShader()->setSkeleton(this->gotSkeleton);

	System::getDeviceContext()->PSSetShaderResources(0, 1, &texture->getTexture());
	if (this->normalMap != nullptr)
	{
		System::getDeviceContext()->PSSetShaderResources(1, 1, &this->normalMap->getTexture());
	}
	if (this->hasGlowMap)
	{
		System::getDeviceContext()->PSSetShaderResources(2, 1, &this->glowMap->getTexture());
	}
	else
	{
		ID3D11ShaderResourceView* temp = nullptr;
		System::getDeviceContext()->PSSetShaderResources(2, 1, &temp);
	}
	if (this->hasMask)
	{
		System::getDeviceContext()->PSSetShaderResources(3, 1, &this->mask->getTexture());
	}
	else
	{
		ID3D11ShaderResourceView* temp = nullptr;
		System::getDeviceContext()->PSSetShaderResources(3, 1, &temp);
	}
	System::getDeviceContext()->IASetVertexBuffers(0, 1, &*this->vertexBuffer.GetAddressOf(), &*vertexBuffer.getStridePtr(), &offset);
//	UINT offset = 0;
	//devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	//System::getDeviceContext()->IASetIndexBuffer(indexBuffer.getBuffer(), DXGI_FORMAT_R32_UINT, offset);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	System::getDeviceContext()->PSSetSamplers(0, 1, &this->SamplerState);

	this->theShader->renderShader((int)mesh.size(),indexBuffer.getBufferSize());
}

void Model::setMaskTexture(shared_ptr<Texture> texture)
{
	this->mask = texture;
	this->hasMask = true;
}

void Model::SetTexture(shared_ptr<Texture> t)
{
	this->texture = t;
	if (texture->isTransparent())
	{
		this->type = Transparent;
	}
	else
	{
		this->type = Opaque;
	}
}
