#include "SimpleShaderClass.h"

bool VertexShader::CreatVertexShader(ID3D11Device* device, std::wstring fileName, D3D11_INPUT_ELEMENT_DESC* layout, UINT elements)
{
	HRESULT hr = D3DReadFileToBlob(fileName.c_str(), this->buffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateVertexShader(this->buffer->GetBufferPointer(), this->buffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{

		return false;
	}

	hr = device->CreateInputLayout(layout, elements, this->buffer->GetBufferPointer(), this->buffer->GetBufferSize(), this->inputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool PixelShader::CreatPixelShader(ID3D11Device* device, std::wstring fileName)
{
	HRESULT hr = D3DReadFileToBlob(fileName.c_str(), this->buffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreatePixelShader(this->buffer.Get()->GetBufferPointer(), this->buffer.Get()->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool SimpleShader::InitializeShaders()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
	};

	UINT numElements = ARRAYSIZE(layout);
	std::wstring shaderfolder = L"..\\x64\\Debug\\";

	CreatVertexShader(System::getDevice(), shaderfolder + L"SimpleVS.cso", layout, numElements);
	CreatPixelShader(System::getDevice(), shaderfolder + L"SimplePS.cso");

	//Create sampler description for sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = System::getDevice()->CreateSamplerState(&sampDesc, &this->samplerState); //Create sampler state
	if (FAILED(hr))
	{
		MessageBox(NULL, "D3D11CreateDevice Failed.",
			"CreateVertexBuffer Error", MB_OK);
		return false;
	}
	return false;
}

bool SimpleShader::CreatGeometryShader(ID3D11Device* device, std::wstring fileName)
{
	if (!this->geometryShader.CreatGeometryShader(device, fileName))
		return false;
	return true;
}

bool SimpleShader::CreatVertexShader(ID3D11Device* device, std::wstring fileName, D3D11_INPUT_ELEMENT_DESC* layout, UINT elements)
{
	if (!this->vertexShader.CreatVertexShader(device, fileName, layout, elements))
		return false;
	return true;
}

bool SimpleShader::CreatPixelShader(ID3D11Device* device, std::wstring fileName)
{
	if (!this->pixdelShader.CreatPixelShader(device, fileName))
		return false;
	return true;
}

bool GeometryShader::CreatGeometryShader(ID3D11Device* device, std::wstring fileName)
{
	HRESULT hr = D3DReadFileToBlob(fileName.c_str(), this->buffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateGeometryShader(this->buffer.Get()->GetBufferPointer(), this->buffer.Get()->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}
