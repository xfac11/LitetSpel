#include"GraphicsDevice.h"
ID3D11Device* GraphicsDevice::device=0;
ID3D11DeviceContext* GraphicsDevice::deviceContext=0;
GraphicsDevice::GraphicsDevice()
{
	this->alphaDisableBlendingState = nullptr;
	this->alphaEnableBlendingState = nullptr;
	this->debug = nullptr;
	this->depthStencilBuffer = nullptr;
	this->depthStencilState = nullptr;
	this->depthStencilView = nullptr;
	this->device = nullptr;
	this->deviceContext = nullptr;
	this->disableDepthStencilState = nullptr;
	
}

GraphicsDevice::~GraphicsDevice()
{
}

void GraphicsDevice::beginScene(float color[4])
{
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0xFF);
	deviceContext->OMSetDepthStencilState(this->depthStencilState, 0); //1
}

void GraphicsDevice::presentScene()
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

bool GraphicsDevice::initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;

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

		D3D11_DEPTH_STENCIL_DESC depthStencilDescL;
		ZeroMemory(&depthStencilDescL, sizeof(depthStencilDescL));
		// Depth test parameters

// Depth test parameters
		depthStencilDescL.DepthEnable = true;
		depthStencilDescL.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDescL.DepthFunc = D3D11_COMPARISON_GREATER;

		// Stencil test parameters
		depthStencilDescL.StencilEnable = false;
		depthStencilDescL.StencilReadMask = 0xFF;
		depthStencilDescL.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		depthStencilDescL.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDescL.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDescL.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT;
		depthStencilDescL.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		depthStencilDescL.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDescL.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDescL.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT;
		depthStencilDescL.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		// Create depth stencil state
		result = device->CreateDepthStencilState(&depthStencilDescL, &disableDepthStencilState);
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

	fieldOfView = 0.45f*DirectX::XM_PI;
	screenAspect = (float)screenWidth / (float)screenHeight;


	//move to ColorShader
	this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 500.f);
	


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

	deviceContext->RSSetState(rasterState);
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
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
	float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };

	// Turn on the alpha blending.
	deviceContext->OMSetBlendState(alphaEnableBlendingState, blendFactor, 0xffffffff);

	this->device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debug));
	return true;
}

void GraphicsDevice::shutDown()
{
	if(alphaDisableBlendingState)
		alphaDisableBlendingState->Release();
	if(alphaEnableBlendingState)
		alphaEnableBlendingState->Release();
	if (this->depthStencilBuffer)
		this->depthStencilBuffer->Release();
	if (this->depthStencilState)
		this->depthStencilState->Release();
	if (this->depthStencilView)
		this->depthStencilView->Release();
	if (this->disableDepthStencilState)
		this->disableDepthStencilState->Release();
	if (this->rasterState)
		this->rasterState->Release();
	if (this->renderTargetView)
		this->renderTargetView->Release();
	if (this->swapChain)
		this->swapChain->Release();
	if (this->deviceContext)
		this->deviceContext->Release();
	debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
	if (this->device)
		this->device->Release();
	if (this->debug)
		this->debug->Release();

	
}

DirectX::XMMATRIX GraphicsDevice::getProj()
{
	return this->projectionMatrix;
}

ID3D11Device *& GraphicsDevice::getDevice()
{
	return device;
}

ID3D11DeviceContext *& GraphicsDevice::getDeviceContext()
{
	return deviceContext;
}
