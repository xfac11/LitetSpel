#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H
#include<d3d11.h>
#include<wrl/client.h>
#include<memory>
#include<vector>
#include"System.h"
class IndexBuffer
{
public:
	IndexBuffer()
	{

	}

	ID3D11Buffer* getBuffer()const
	{
		return buffer.Get();
	}

	ID3D11Buffer* const* getAddressOfBuffer()const
	{
		return buffer.GetAddressOf();
	}

	UINT getBufferSize()const
	{
		return bufferSize;
	}

	HRESULT initialize(DWORD * data, UINT numIndices)
	{
		bufferSize = numIndices;

		D3D11_BUFFER_DESC iBufferDesc;
		ZeroMemory(&iBufferDesc, sizeof(iBufferDesc));

		iBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		iBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		iBufferDesc.ByteWidth = sizeof(DWORD) * numIndices;
		iBufferDesc.CPUAccessFlags = 0;
		iBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(subData));
		subData.pSysMem = data;

		HRESULT hr = System::getDevice()->CreateBuffer(&iBufferDesc, &subData, buffer.GetAddressOf());

		return hr;
	}
private:
	IndexBuffer(const IndexBuffer& obj);
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT bufferSize = 0;


};
#endif // !INDEXBUFFER_H
