#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H
#include<d3d11.h>
#include<wrl/client.h>
#include<memory>
#include<vector>
#include"System.h"
template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer()
	{

	}

	T data;

	ID3D11Buffer* getBuffer()const
	{
		return buffer.Get();
	}

	ID3D11Buffer* const* getAddressOfBuffer()const
	{
		return buffer.GetAddressOf();
	}

	HRESULT initialize()
	{
		D3D11_BUFFER_DESC desc;

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		desc.StructureByteStride = 0;

		HRESULT hr = System::getDevice()->CreateBuffer(&desc, 0, buffer.GetAddressOf());
		return hr;
	}

	bool applyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = System::getDeviceContext()->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		if (FAILED(hr))
		{
			return false;
		}

		CopyMemory(mappedResource.pData, &data, sizeof(T));
		System::getDeviceContext()->Unmap(buffer.Get(), 0);

		return true;
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
};
#endif // !CONSTANTBUFFER_H
