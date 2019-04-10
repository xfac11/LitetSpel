#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <iostream>

using Microsoft::WRL::ComPtr;

class VertexShader
{
public:
	bool CreatVertexShader(ID3D11Device* device, std::wstring fileName, D3D11_INPUT_ELEMENT_DESC * layout, UINT elements);
	ID3D11VertexShader *GetShader() { return shader.Get(); }
	ID3D10Blob *GetBuffer() { return buffer.Get(); }
	ID3D11InputLayout * GetInputLayout() { return inputLayout.Get(); }
private:
	ComPtr<ID3D11VertexShader> shader = nullptr;
	ComPtr<ID3D10Blob> buffer = nullptr;
	ComPtr<ID3D11InputLayout> inputLayout = nullptr;
};

class GeometryShader
{
public:
	bool CreatGeometryShader(ID3D11Device* device, std::wstring fileName);
	ID3D11GeometryShader * GetShader() { return shader.Get(); }
	ID3D10Blob * GetBuffer() { return buffer.Get(); }
private:
	ComPtr<ID3D11GeometryShader> shader;
	ComPtr<ID3D10Blob> buffer;
};

class PixelShader
{
public:
	bool CreatPixelShader(ID3D11Device* device, std::wstring fileName);
	ID3D11PixelShader * GetShader() { return shader.Get(); }
	ID3D10Blob * GetBuffer() { return buffer.Get(); }
private:
	ComPtr<ID3D11PixelShader> shader;
	ComPtr<ID3D10Blob> buffer;
};


class Shader
{
public:
	bool CreatGeometryShader(ID3D11Device* device, std::wstring fileName);
	bool CreatVertexShader(ID3D11Device* device, std::wstring fileName, D3D11_INPUT_ELEMENT_DESC * layout, UINT elements);
	bool CreatPixelShader(ID3D11Device* device, std::wstring fileName);
	VertexShader * GetVertexShader() { return &vertexShader;}
	PixelShader * GetPixelShader() { return &pixdelShader; }
	GeometryShader * GetGeometryShader() { return &geometryShader; }
private:
	VertexShader vertexShader;
	PixelShader pixdelShader;
	GeometryShader geometryShader;
};