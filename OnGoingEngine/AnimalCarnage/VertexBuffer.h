#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
#include<d3d11.h>
#include<wrl/client.h>
#include<memory>
#include"System.h"
template<class T>
class VertexBuffer
{
public:
	VertexBuffer()
	{

	}

	ID3D11Buffer* Get()const
	{
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf()const
	{
		return buffer.GetAddressOf();
	}

	UINT getBufferSize()const
	{
		return this->bufferSize;
	}

	const UINT getStride()const
	{
		return *stride.get();
	}

	const UINT* getStridePtr()const
	{
		return stride.get();
	}

	HRESULT initialize(T* data, UINT numberOfVertices)
	{
		bufferSize = numberOfVertices;
		stride = std::make_unique<UINT>(sizeof(T));

		D3D11_BUFFER_DESC vBufferDesc;
		ZeroMemory(&vBufferDesc, sizeof(vBufferDesc));

		vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vBufferDesc.ByteWidth = sizeof(T) * numberOfVertices;
		vBufferDesc.CPUAccessFlags = 0;
		vBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(subData));
		subData.pSysMem = data;

		HRESULT hr = System::getDevice()->CreateBuffer(&vBufferDesc, &subData, buffer.GetAddressOf());

		return hr;
	}

private:
	VertexBuffer(const VertexBuffer<T>& rhs);

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	std::unique_ptr<UINT> stride;
	UINT bufferSize = 0;


};
#endif // !VERTEXBUFFER_H
