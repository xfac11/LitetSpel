#include"GBuffer.h"

GBuffer::GBuffer()
{
	for (int i = 0; i < GBUFFERCAP; i++)
	{
		this->texTures[i] = nullptr;
		this->renderTars[i] = nullptr;
		this->shaderResViews[i] = nullptr;
	}
	depthSource = nullptr;
	depthBuffer = nullptr;
	depthStencView = nullptr;
}

GBuffer::~GBuffer()
{

}

bool GBuffer::initialize(ID3D11Device * device, int height, int width, float nearPlane, float farPlane)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC texDesc{};
	D3D11_RENDER_TARGET_VIEW_DESC renTarViewDesc{};
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResViewDesc{};
	D3D11_TEXTURE2D_DESC depthBufferDesc{};
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencViewDesc{};

	this->height = height;
	this->width = width;

	ZeroMemory(&texDesc, sizeof(texDesc));

	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.ArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.CPUAccessFlags = 0;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;

	for (int i = 0; i < GBUFFERCAP; i++)
	{
		result = device->CreateTexture2D(&texDesc, NULL, &this->texTures[i]);
		if (FAILED(result))
			return false;

	}

	renTarViewDesc.Format = texDesc.Format;
	renTarViewDesc.Texture2D.MipSlice = 0;
	renTarViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	for (int i = 0; i < GBUFFERCAP; i++)
	{
		result = device->CreateRenderTargetView(this->texTures[i], &renTarViewDesc, &this->renderTars[i]);
		if (FAILED(result))
			return false;

	}
	shaderResViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResViewDesc.Texture2D.MipLevels = 1;
	shaderResViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResViewDesc.Format = texDesc.Format;

	for (int i = 0; i < GBUFFERCAP; i++)
	{
		result = device->CreateShaderResourceView(this->texTures[i], &shaderResViewDesc, &this->shaderResViews[i]);
		if (FAILED(result))
			return false;

	}

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthBuffer);
	if (FAILED(result))
	{
		return false;
	}

	ZeroMemory(&depthStencViewDesc, sizeof(depthStencViewDesc));

	depthStencViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(depthBuffer, &depthStencViewDesc, &depthStencView);
	if (FAILED(result))
	{
		return false;
	}
	//result=device->CreateShaderResourceView(depthBuffer, &shaderResViewDesc, &this->depthSource);
	if (FAILED(result))
	{
		return false;
	}
	viewP.Width = float(width);
	viewP.Height = float(height);
	viewP.MinDepth = 0.0f;
	viewP.MaxDepth = 1.0f;
	viewP.TopLeftX = 0.0f;
	viewP.TopLeftY = 0.0f;


	return true;
}

bool GBuffer::setRenderTargets(ID3D11Device *& device, ID3D11DeviceContext * deviceContext)
{
	ID3D11ShaderResourceView* null[] = { nullptr, nullptr, nullptr };
	deviceContext->PSSetShaderResources(0, 3, null);
	deviceContext->OMSetRenderTargets(GBUFFERCAP, this->renderTars, depthStencView);
	deviceContext->RSSetViewports(1, &viewP);
	return true;//always returns true
}

ID3D11RenderTargetView*& GBuffer::getRenView(int id)
{
	return this->renderTars[id];
}

void GBuffer::shutDown()
{

	for (int i = 0; i < GBUFFERCAP; i++)
	{
		this->texTures[i]->Release();
		this->renderTars[i]->Release();
		this->shaderResViews[i]->Release();
	}

	if (this->depthBuffer) 
	{
		this->depthBuffer->Release();
	}
	if (this->depthStencView)
	{
		this->depthStencView->Release();
	}
	
	if (this->depthSource)
	{
		depthSource->Release();
	}
}

void GBuffer::clear(ID3D11DeviceContext * deviceContext, float color[4])
{
	for (int i = 0; i < GBUFFERCAP; i++)
	{
		deviceContext->ClearRenderTargetView(this->renderTars[i], color);
	}
	deviceContext->ClearDepthStencilView(depthStencView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void GBuffer::setShaderResViews(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetShaderResources(0, GBUFFERCAP, this->shaderResViews);
}

ID3D11DepthStencilView *& GBuffer::getDepthStcView()
{
	return this->depthStencView;
}

ID3D11ShaderResourceView* GBuffer::getShadResView(int id)
{
	return this->shaderResViews[id];
}

ID3D11Texture2D * GBuffer::getTexture(int id)
{
	return this->texTures[id];
}