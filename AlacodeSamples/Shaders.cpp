#include "Shaders.h"


bool VertexShader::CreatVertexShader(ID3D11Device* device, std::wstring fileName, D3D11_INPUT_ELEMENT_DESC * layout, UINT elements)
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

bool PixelShader::CreatPixelShader(ID3D11Device * device, std::wstring fileName)
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

bool Shader::CreatGeometryShader(ID3D11Device * device, std::wstring fileName)
{
	if (!this->geometryShader.CreatGeometryShader(device, fileName))
		return false;
	return true;
}

bool Shader::CreatVertexShader(ID3D11Device * device, std::wstring fileName, D3D11_INPUT_ELEMENT_DESC * layout, UINT elements)
{
	if (!this->vertexShader.CreatVertexShader(device, fileName, layout, elements))
		return false;
	return true;
}

bool Shader::CreatPixelShader(ID3D11Device * device, std::wstring fileName)
{
	if (!this->pixdelShader.CreatPixelShader(device, fileName))
		return false;
	return true;
}

bool GeometryShader::CreatGeometryShader(ID3D11Device * device, std::wstring fileName)
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
