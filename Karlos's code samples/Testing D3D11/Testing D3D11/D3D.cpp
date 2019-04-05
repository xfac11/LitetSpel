#include "D3D.h"

D3D::D3D()
{
	this->vSync_enabled = NULL;
	this->videoCardMemory = NULL;
	this->videoCardDescription[128] = NULL;

	this->swapChain =  nullptr;
	this->device = nullptr;
	this->deviceContext = nullptr;
	this->renderTargetView = nullptr;
	this->depthStencilBuffer = nullptr;
	this->depthStencilState = nullptr;
	this->disableDepthStencilState = nullptr;
	this->depthStencilView = nullptr;
	this->rasterState = nullptr;
	this->alphaEnableBlendingState = nullptr;
	this->alphaDisableBlendingState = nullptr;
	
	this->debug = nullptr;

	this->dist = 0.1f;
	this->gIncrement = 0;
}

D3D::~D3D()
{
}
void D3D::setIncrement(float g)
{
	this->gIncrement = g;
}

bool D3D::initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear) 
{
	HRESULT result;
	//IDXGIFactory* factory;
	//IDXGIAdapter* adapter;
	//IDXGIOutput* adapterOutput;
	//unsigned int numModes, i, numerator, denominator;
	//unsigned long long stringLength;
	//DXGI_MODE_DESC* displayModeList;
	//DXGI_ADAPTER_DESC adapterDesc;
	
	//DXGI_SWAP_CHAIN_DESC swapchainDesc;
	//D3D_FEATURE_LEVEL featureLevel;
	//ID3D11Texture2D* backBufferPtr = nullptr;
	//D3D11_TEXTURE2D_DESC depthBufferDesc;
	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc; //dno if we need this

	D3D11_BLEND_DESC blendStateDescription;
	float fieldOfView;
	float screenAspect;

	vSync_enabled = vsync;

	DXGI_SWAP_CHAIN_DESC swapchainDesc;
	ZeroMemory(&swapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	//swapchainDesc.BufferDesc.Width = 0;
	//swapchainDesc.BufferDesc.Height = 0; //defualt to window size
	swapchainDesc.BufferCount = 1;
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.OutputWindow = hwnd;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.Windowed = TRUE;

	//if (vSync_enabled)
	//{
	//	swapchainDesc.BufferDesc.RefreshRate.Numerator = numerator;
	//	swapchainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	//}
	//else
	//{
	//	swapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
	//	swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
	//}

	result = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapchainDesc,
		&swapChain,
		&device,
		NULL,
		&deviceContext);

	if (SUCCEEDED(result))
	{
		// get the address of the back buffer
		ID3D11Texture2D* backBufferPtr = nullptr;
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if (FAILED(result))
		{
			//result = false;
			MessageBox(hwnd, "Could not ID3D11Texture2D* backBufferPtr", "Error", MB_OK); //L"", L"", ;
			return false;
		}

		// use the back buffer address to create the render target
		device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
		if (FAILED(result))
		{
			//result = false;
			MessageBox(hwnd, "Could not ID3D11Texture2D* backBufferPtr", "Error", MB_OK);
			return false;
		}
		backBufferPtr->Release();
		backBufferPtr = nullptr;

		
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = (UINT)screenWidth;
		descDepth.Height = (UINT)screenHeight;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //DXGI_FORMAT_R32_TYPELESS;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;// | D3D10_BIND_SHADER_RESOURCE;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		result = device->CreateTexture2D(&descDepth, NULL, &depthStencilBuffer);
		if (FAILED(result))
		{
			return false;
		}
		result = device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView); // &depthStencilViewDesc was NULL
		if (FAILED(result))
		{
			// deal with error...
			return false;
		}
		deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

		//the depth Stencil State
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		// Depth test parameters
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		// Create depth stencil state
		result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
		if (FAILED(result))
		{
			// deal with error...
			return false;
		}

		//dis
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		// Depth test parameters
		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		// Create depth stencil state
		result = device->CreateDepthStencilState(&depthStencilDesc, &disableDepthStencilState);
		if (FAILED(result))
		{
			// deal with error...
			return false;
		}


	}

	//void SetViewport()
	this->vp.Width = (float)screenWidth;
	this->vp.Height = (float)screenHeight;
	this->vp.MinDepth = 0.0f;
	this->vp.MaxDepth = 1.0f;
	this->vp.TopLeftX = 0;
	this->vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1, &this->vp);

	//do this later
	//this->gIncrement += 0.8f * ImGui::GetIO().DeltaTime; 
	//gConstantBufferData->offset = sin(this->gIncrement);

	//fieldOfView = 3.141592654f / 4.0f; //0.785
	fieldOfView = 0.45f*DirectX::XM_PI;
	screenAspect = (float)screenWidth / (float)screenHeight;


	//move to ColorShader
	this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 500.f);
	//this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	//this->worldMatrix = DirectX::XMMatrixIdentity();
	//this->worldMatrix = DirectX::XMMatrixRotationY(this->gIncrement);
	//this->worldMatrix = DirectX::XMMatrixTranspose(worldMatrix); //moved to colorshader
	//this->orthoMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);


	D3D11_RASTERIZER_DESC rasterizerDesc; 
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

	result = device->CreateRasterizerState(&rasterizerDesc, &rasterState);
	if (FAILED(result)) //If error occurred
	{
		MessageBox(NULL, "Failed to create rasterizer state.",
			"D3D11 Initialisation Error", MB_OK);
		return false;
	}


	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = device->CreateBlendState(&blendStateDescription, &alphaEnableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	result = device->CreateBlendState(&blendStateDescription, &alphaDisableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	this->device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debug));
	return true;
}

void D3D::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (swapChain)
	{
		swapChain->SetFullscreenState(false, NULL);
	}
	if (rasterState)
	{
		rasterState->Release();
		rasterState = nullptr;
	}
	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = nullptr;
	}
	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = nullptr;
	}
	if (disableDepthStencilState)
	{
		disableDepthStencilState->Release();
		disableDepthStencilState = nullptr;
	}
	if (depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = nullptr;
	}
	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = nullptr;
	}
	if (deviceContext)
	{
		deviceContext->Release();
		deviceContext = nullptr;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = nullptr;
	}
	if (alphaEnableBlendingState)
	{
		alphaEnableBlendingState->Release();
		alphaEnableBlendingState = nullptr;
	}

	if (alphaDisableBlendingState)
	{
		alphaDisableBlendingState->Release();
		alphaDisableBlendingState = nullptr;
	}
	debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
	if (device)
	{
		device->Release();
		device = nullptr;
	}
	
	if (debug)
	{
		debug->Release();
		debug = nullptr;
	}

}

void D3D::BeginScene(float color[4])
{
	//float color[4];
	// Setup the color to clear the buffer to.
	//color[0] = red;
	//color[1] = green;
	//color[2] = blue;
	//color[3] = alpha;
	
	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext->OMSetDepthStencilState(this->depthStencilState, 0); //1
	this->worldMatrix = DirectX::XMMatrixRotationY(this->gIncrement);
}

void D3D::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (vSync_enabled == true)
	{
		// Lock to screen refresh rate.
		swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		swapChain->Present(0, 0);
	}
}

void D3D::setBackBuffer()
{
	deviceContext->OMSetRenderTargets(1, &renderTargetView, this->depthStencilView);
}
void D3D::setBackBuffer(ID3D11DepthStencilView* view)
{
	deviceContext->OMSetRenderTargets(1, &renderTargetView, view);
}

void D3D::turnOffZ()
{
	deviceContext->OMSetDepthStencilState(this->disableDepthStencilState, 1); //1
}

void D3D::turnOnZ()
{
	deviceContext->OMSetDepthStencilState(this->depthStencilState, 1); //1
}

void D3D::resetViewPort()
{
	deviceContext->RSSetViewports(1, &vp);
}

void D3D::EnableAlphaBlending()
{
	// Setup the blend factor.
	float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };

	// Turn on the alpha blending.
	deviceContext->OMSetBlendState(alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void D3D::DisableAlphaBlending()
{
	// Setup the blend factor.
	float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };

	// Turn off the alpha blending. 
	//disable is nullptr
	deviceContext->OMSetBlendState(alphaDisableBlendingState, blendFactor, 0xffffffff);
}

ID3D11Device *& D3D::GetDevice()
{
	return device;
}

ID3D11DeviceContext *& D3D::GetDeviceContext()
{
	return deviceContext;
}

DirectX::XMMATRIX& D3D::GetProjectionMatrix()
{
	return projectionMatrix;
}

DirectX::XMMATRIX & D3D::GetWorldMatrix()
{
	return  worldMatrix;
}

int & D3D::GetVideoCardInfo(char * cardName)//, int & memory)
{
	strcpy_s(cardName, 128, this->videoCardDescription);
	return videoCardMemory;
}
