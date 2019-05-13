#include "LightShader.h"
#include"System.h"


LightShader::LightShader():
	Shader()
{
	this->blendState = nullptr;
	this->rasState = nullptr;
	
}


LightShader::~LightShader()
{
	this->shutdown();
}

bool LightShader::initialize()
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
	if (!this->load(L"LSVertexShader.hlsl", inputDesc, ARRAYSIZE(inputDesc), L"LSPixelShader.hlsl"))
	{
		return false;
	}
	result = this->perFrameCB.initialize(System::getDevice());
	if (FAILED(result))
	{
		return false;
	}
	result = this->worldCB.initialize(System::getDevice());
	if (FAILED(result))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDescL;
	ZeroMemory(&depthStencilDescL, sizeof(depthStencilDescL));
	// Depth test parameters

// Depth test parameters
	depthStencilDescL.DepthEnable = true;
	depthStencilDescL.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;//ALL
	depthStencilDescL.DepthFunc = D3D11_COMPARISON_GREATER;

	// Stencil test parameters
	depthStencilDescL.StencilEnable = true;
	depthStencilDescL.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDescL.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	// Stencil operations if pixel is front-facing
	depthStencilDescL.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescL.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;//decrement the value in the stencil buffer when the depth test fails
	depthStencilDescL.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT;
	depthStencilDescL.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	depthStencilDescL.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescL.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;//increment the value in the stencil buffer when the depth test fails
	depthStencilDescL.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT;
	depthStencilDescL.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Create depth stencil state
	result = System::getDevice()->CreateDepthStencilState(&depthStencilDescL, &depthStencilState);
	if (FAILED(result))
	{
		// deal with error...
		return false;
	}
	depthStencilDescL.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDescL.DepthEnable = false;
	depthStencilDescL.StencilEnable = false;

	result = System::getDevice()->CreateDepthStencilState(&depthStencilDescL, &dpthQuad);
	if (FAILED(result))
	{
		// deal with error...
		return false;
	}
	//second pass
	D3D11_DEPTH_STENCIL_DESC disDepthStencilDescL;
	ZeroMemory(&disDepthStencilDescL, sizeof(disDepthStencilDescL));
	// Depth test parameters

// Depth test parameters
	disDepthStencilDescL.DepthEnable = true;
	disDepthStencilDescL.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;//ALL
	disDepthStencilDescL.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;

	disDepthStencilDescL.StencilEnable = true;
	disDepthStencilDescL.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	disDepthStencilDescL.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	// Stencil operations if pixel is front-facing
	disDepthStencilDescL.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	disDepthStencilDescL.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;//decrement the value in the stencil buffer when the depth test fails
	disDepthStencilDescL.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	disDepthStencilDescL.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	// Stencil operations if pixel is back-facing
	disDepthStencilDescL.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	disDepthStencilDescL.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;//increment the value in the stencil buffer when the depth test fails
	disDepthStencilDescL.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	disDepthStencilDescL.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	result = System::getDevice()->CreateDepthStencilState(&disDepthStencilDescL, &disDepthStencilState);
	if (FAILED(result))
	{
		// deal with error...
		return false;
	}
	//First pass
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	rasterizerDesc.DepthClipEnable = true;
	result = System::getDevice()->CreateRasterizerState(&rasterizerDesc, &rasState);
	if (FAILED(result)) //If error occurred
	{
		MessageBox(NULL, "Failed to create rasterizer state.",
			"D3D11 Initialisation Error", MB_OK);
		return false;
	}
	//Second pass
	//D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

	rasterizerDesc.DepthClipEnable = false;
	result = System::getDevice()->CreateRasterizerState(&rasterizerDesc, &fRasState);
	if (FAILED(result)) //If error occurred
	{
		MessageBox(NULL, "Failed to create rasterizer state.",
			"D3D11 Initialisation Error", MB_OK);
		return false;
	}


	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = true;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// Create the blend state using the description.
	result = System::getDevice()->CreateBlendState(&blendStateDescription, &blendState);
	if (FAILED(result))
	{
		return false;
	}


	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = System::getDevice()->CreateSamplerState(&desc, &this->sampler);



	return true;
	//Rasteriser state, blend state, depthstencil state is in GraphicsDevice. 
	//Like a default. The rasteriser state is set in the initialize of GraphicsDevice
}

void LightShader::setWorld(DirectX::XMMATRIX world)
{
	//world = XMMatrixTranspose(world);
	this->worldCB.data.world = world;
	this->worldCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void LightShader::setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 camPos)
{
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);
	this->perFrameCB.data.view = view;
	this->perFrameCB.data.proj = proj;
	//need to set campos separately to enable check for backface culling
	//this->perFrameCB.data.camPos = camPos; 
	this->perFrameCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void LightShader::setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos)
{
	XMFLOAT4 cam = { campos.x, campos.y,campos.z,1.f };
	perFrameCB.data.camPos = cam;
}

void LightShader::setCBuffers()
{
	this->setConstanbuffer(VERTEX, 0, this->perFrameCB.getBuffer());
	this->setConstanbuffer(PIXEL, 0, this->perFrameCB.getBuffer());
	this->setConstanbuffer(VERTEX, 1, this->worldCB.getBuffer());
}

void LightShader::renderShaderDir(int vertexCount)
{
	float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };

	//System::getDeviceContext()->OMSetDepthStencilState(dpthQuad, 0);
	//System::getDeviceContext()->PSSetSamplers(0, 1, &sampler);
	//System::getDeviceContext()->RSSetState(rasState);
	System::getDeviceContext()->OMSetBlendState(blendState, blendFactor, 1);
	System::shaderManager->getShadowMapping()->setSampler();
	this->renderPixels(vertexCount);//shade the pixels
}

void LightShader::renderShaderPoint(int indexCount, ID3D11DepthStencilView * view)
{
	float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };

	System::getDeviceContext()->OMSetDepthStencilState(depthStencilState, 0);
	System::getDeviceContext()->ClearDepthStencilView(view, D3D11_CLEAR_STENCIL, 1.0f, 1);
	System::getDeviceContext()->RSSetState(rasState);
	this->renderUnmark(indexCount);//unmark all of the pixels that are in front of the front faces of the light’s geometric volume
	System::getDeviceContext()->OMSetBlendState(blendState, blendFactor, 1);
	System::getDeviceContext()->OMSetDepthStencilState(disDepthStencilState, 1);
	System::getDeviceContext()->RSSetState(fRasState);

	System::getDeviceContext()->VSSetShader(this->vertexShader, nullptr, 0);
	System::getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->PSSetShader(this->pixelShader, nullptr, 0);
	System::getDeviceContext()->IASetInputLayout(this->vertexLayout);
	System::getDeviceContext()->PSSetSamplers(0, 1, &sampler);
	System::getDeviceContext()->DrawIndexed(indexCount, 0, 0);
	//System::getDeviceContext()->Draw(count, 0);
	System::getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
}

void LightShader::setTypeOfLight(int type)
{
	this->perFrameCB.data.camPos.w = type;
	this->perFrameCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void LightShader::renderUnmark(int count)
{
	System::getDeviceContext()->VSSetShader(this->vertexShader, nullptr, 0);
	System::getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->PSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->IASetInputLayout(this->vertexLayout);
	System::getDeviceContext()->PSSetSamplers(0, 1, &sampler);
	System::getDeviceContext()->DrawIndexed(count, 0,0);
	//System::getDeviceContext()->Draw(count, 0);
	System::getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
}

void LightShader::renderPixels(int count)
{
	this->renderShader(count, count);
}

void LightShader::shutdown()
{
	if (this->blendState != nullptr)
		this->blendState->Release();
	if (this->rasState != nullptr)
		this->rasState->Release();
	if (this->depthStencilState != nullptr)
		this->depthStencilState->Release();
	if (this->disDepthStencilState != nullptr)
		this->disDepthStencilState->Release();
	if (this->fRasState != nullptr)
		this->fRasState->Release();
	if (this->dpthQuad != nullptr)
		this->dpthQuad->Release();
	if (this->sampler != nullptr)
		this->sampler->Release();
}
