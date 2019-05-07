#include "ShadowMapping.h"
#include"System.h"


ShadowMapping::ShadowMapping()
{
}


ShadowMapping::~ShadowMapping()
{
	this->shutdown();
}

bool ShadowMapping::initialize()
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"SV_POSITION",		// "semantic" name in shader
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
		},

		{
			"NORMAL",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
			//for normal mapping. tangent binormal
		   },
		   {
			   "TANGENT", //normal maps
			   0,
			   DXGI_FORMAT_R32G32B32_FLOAT,
			   0,
			   D3D11_APPEND_ALIGNED_ELEMENT,
			   D3D11_INPUT_PER_VERTEX_DATA,
			   0
		   },
		   {
			   "BINORMAL", //normal maps
			   0,
			   DXGI_FORMAT_R32G32B32_FLOAT,
			   0,
			   D3D11_APPEND_ALIGNED_ELEMENT,
			   D3D11_INPUT_PER_VERTEX_DATA,
			   0
		   }


	};
	HRESULT result;
	if (!this->load(L"MulVertexShader.hlsl", inputDesc, ARRAYSIZE(inputDesc)))
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


	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = System::getWindowArea().width;
	depthStencilDesc.Height = System::getWindowArea().height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	dsv_desc.Flags = 0;
	dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
	sr_desc.Format = DXGI_FORMAT_R32_FLOAT;
	sr_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	sr_desc.Texture2D.MostDetailedMip = 0;
	sr_desc.Texture2D.MipLevels = 1;

	if (FAILED(System::getDevice()->CreateTexture2D(&depthStencilDesc, 0, &depthTexture)))
		return false;
	if (FAILED(System::getDevice()->CreateDepthStencilView(depthTexture, &dsv_desc, &depthStencilView)))
		return false;
	if (FAILED(System::getDevice()->CreateShaderResourceView(depthTexture, &sr_desc, &depthShaderResource)))
		return false;


	return true;
}

void ShadowMapping::setWorld(DirectX::XMMATRIX world)
{
	world = XMMatrixTranspose(world);
	this->worldCB.data.world = world;
	this->worldCB.applyChanges(System::getDevice(),System::getDeviceContext());
}

void ShadowMapping::setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 camPos)
{
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);
	this->perFrameCB.data.proj = proj;
	this->perFrameCB.data.view = view;
	this->perFrameCB.data.camPos = camPos;
	this->perFrameCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void ShadowMapping::setView(DirectX::XMMATRIX view)
{
	view = XMMatrixTranspose(view);
	this->perFrameCB.data.view = view;
	this->perFrameCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void ShadowMapping::setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos)
{
	this->perFrameCB.data.camPos = DirectX::XMFLOAT4(campos.x, campos.y, campos.z, 1.0f);
}

void ShadowMapping::setCBuffers()
{
	this->setConstanbuffer(VERTEX, 0, this->perFrameCB.getBuffer());
	this->setConstanbuffer(VERTEX, 1, this->worldCB.getBuffer());
}

void ShadowMapping::prepare()
{

	this->setShaders();
	System::getDeviceContext()->ClearDepthStencilView(this->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	System::getDeviceContext()->OMSetRenderTargets(0, nullptr, this->depthStencilView);
	//System::getDeviceContext().omset
}

ID3D11ShaderResourceView *& ShadowMapping::getShadowMap()
{
	return this->depthShaderResource;
}

void ShadowMapping::shutdown()
{
	if(this->depthShaderResource!=nullptr)
		this->depthShaderResource->Release();
	if (this->depthStencilView != nullptr)
		this->depthStencilView->Release();
	if (this->depthTexture != nullptr)
		this->depthTexture->Release();

}
