#include "VerticalBlur.h"
#include"System.h"


VerticalBlur::VerticalBlur():
	Shader()
{
	this->renderTarget = nullptr;
	this->texture = nullptr;
	this->shaderView = nullptr;
}


VerticalBlur::~VerticalBlur()
{
	if (this->renderTarget != nullptr)
		this->renderTarget->Release();
	if (this->texture != nullptr)
		this->texture->Release();
	if (this->shaderView != nullptr)
		this->shaderView->Release();
	
}

bool VerticalBlur::initialize(int height, int width)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
			{
				"POSITION",		// "semantic" name in shader
				0,				// "semantic" index (not used)
				DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
				0,							 // input slot
				D3D11_APPEND_ALIGNED_ELEMENT, // offset of first element
				D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
				0							 // used for INSTANCING (ignore)
			},
			{
				"TEXCOORD",
				0,
				DXGI_FORMAT_R32G32_FLOAT, //2 values
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			}

	};
	HRESULT result;
	if (!this->load(L"VerticalBlurVS.hlsl", inputDesc, ARRAYSIZE(inputDesc), L"VerticalBlurPS.hlsl"))
	{
		return false;
	}
	
	result = this->windowCB.initialize(System::getDevice());
	if (FAILED(result))
	{
		return false;
	}
	this->windowCB.data.height = height;
	this->windowCB.data.width = width;
	this->windowCB.applyChanges(System::getDevice(), System::getDeviceContext());
	//HRESULT result;
	D3D11_TEXTURE2D_DESC texDesc{};
	D3D11_RENDER_TARGET_VIEW_DESC renTarViewDesc{};
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResViewDesc{};
	ZeroMemory(&texDesc, sizeof(texDesc));
	/*float4 Normal : SV_Target0;
	float4 TexColor : SV_Target1;
	float4 Pos : SV_Target2;*/
	texDesc.Width = System::getWindowArea().width;
	texDesc.Height = System::getWindowArea().height;
	texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc.ArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.CPUAccessFlags = 0;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;


	result = System::getDevice()->CreateTexture2D(&texDesc, NULL, &this->texture);
	if (FAILED(result))
		return false;

	renTarViewDesc.Format = texDesc.Format;
	renTarViewDesc.Texture2D.MipSlice = 0;
	renTarViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	result = System::getDevice()->CreateRenderTargetView(this->texture, &renTarViewDesc, &this->renderTarget);
	if (FAILED(result))
		return false;

	
	shaderResViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResViewDesc.Texture2D.MipLevels = 1;
	shaderResViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResViewDesc.Format = texDesc.Format;

	result = System::getDevice()->CreateShaderResourceView(this->texture, &shaderResViewDesc, &this->shaderView);
	if (FAILED(result))
		return false;


	//result=System::getDevice()->CreateShaderResourceView(depthBuffer, &shaderResViewDesc, &this->depthSource);
	if (FAILED(result))
	{
		return false;
	}
	view.Width = float(texDesc.Width);
	view.Height = float(texDesc.Height);
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = System::getDevice()->CreateSamplerState(&desc, &this->sampler);

	return true;
}

bool VerticalBlur::resize(int height, int width)
{
	this->windowCB.data.height = height;
	this->windowCB.data.width = width;
	this->windowCB.applyChanges(System::getDevice(), System::getDeviceContext());

	this->texture->Release();
	this->renderTarget->Release();
	this->shaderView->Release();

	HRESULT result;
	D3D11_TEXTURE2D_DESC texDesc{};
	D3D11_RENDER_TARGET_VIEW_DESC renTarViewDesc{};
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResViewDesc{};
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = System::getWindowArea().width;
	texDesc.Height = System::getWindowArea().height;
	texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc.ArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.CPUAccessFlags = 0;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;

	result = System::getDevice()->CreateTexture2D(&texDesc, NULL, &this->texture);
	if (FAILED(result))
		return false;

	renTarViewDesc.Format = texDesc.Format;
	renTarViewDesc.Texture2D.MipSlice = 0;
	renTarViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	result = System::getDevice()->CreateRenderTargetView(this->texture, &renTarViewDesc, &this->renderTarget);
	if (FAILED(result))
		return false;

	shaderResViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResViewDesc.Texture2D.MipLevels = 1;
	shaderResViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResViewDesc.Format = texDesc.Format;

	result = System::getDevice()->CreateShaderResourceView(this->texture, &shaderResViewDesc, &this->shaderView);
	if (FAILED(result))
		return false;

	view.Width = float(texDesc.Width);
	view.Height = float(texDesc.Height);
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;

	return true;
}

void VerticalBlur::render(int vertexCount, ID3D11ShaderResourceView* shaderView)
{
	System::getDeviceContext()->OMSetRenderTargets(1, &this->renderTarget, nullptr);
	System::getDeviceContext()->PSSetSamplers(0, 1, &this->sampler);
	System::getDeviceContext()->PSSetShaderResources(0, 1, &shaderView);// the one being blurred
	System::getDeviceContext()->RSSetViewports(1, &this->view);
	this->renderShader(vertexCount, 0);
}

void VerticalBlur::setWorld(DirectX::XMMATRIX world)
{
	
}

void VerticalBlur::setCBuffers()
{
	this->setConstanbuffer(VERTEX, 0, this->windowCB.getBuffer());
}

ID3D11ShaderResourceView *& VerticalBlur::getShaderView()
{
	return this->shaderView;// the one that has been blurred
}
