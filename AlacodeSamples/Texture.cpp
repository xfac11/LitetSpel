#include "Texture.h"

bool Texture::LoadTextureFromFile(ID3D11Device * device, std::string filePath, textureType type)
{
	this->type = type;
	this->filePath = filePath;
	std::wstring wide_string(filePath.begin(), filePath.end());
	HRESULT hr = DirectX::CreateWICTextureFromFile(device, wide_string.c_str(), nullptr, myTextureView.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBox(NULL, "Textures Failed.",
			filePath.c_str(), MB_OK);
		return false;
	}
	return true;
}
bool Texture::LoadDefaultTexture(ID3D11Device * device, textureType type)
{
	
		//change color of the pixel
		//https://convertingcolors.com/hex-color-C99AFF.html
		static const uint32_t s_pixel = 0xffFC87D4;

		D3D11_SUBRESOURCE_DATA initData = { &s_pixel, sizeof(uint32_t), 0 };

		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = desc.Height = desc.MipLevels = desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		ComPtr<ID3D11Texture2D> tex;
		HRESULT hr = device->CreateTexture2D(&desc, &initData, tex.GetAddressOf());

		if (SUCCEEDED(hr))
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
			SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MipLevels = 1;

			hr = device->CreateShaderResourceView(tex.Get(),
				&SRVDesc, myTextureView.GetAddressOf());
		}

		if (FAILED(hr))
		{
			// error!
			return false;
		}

	
	return true;
}
int Texture::getType()
{
	return this->type;
}

ID3D11ShaderResourceView ** Texture::getTextureView()
{
	return myTextureView.GetAddressOf();
}

Texture::~Texture()
{
}
