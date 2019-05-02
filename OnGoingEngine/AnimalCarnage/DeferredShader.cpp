#include "DeferredShader.h"
#include"System.h"

DeferredShader::DeferredShader() :
	Shader()
{

}


DeferredShader::~DeferredShader()
{
	if (this->blendState != nullptr)
		this->blendState->Release();
	if (this->rasState != nullptr)
	{
		this->rasState->Release();
	}
}

bool DeferredShader::initialize(int height, int width, float nearPlane, float farPlane)
{
	this->gBuffer.initialize(height, width, nearPlane, farPlane);

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
	if (!this->load(L"DRGBVertexShader.hlsl", inputDesc, ARRAYSIZE(inputDesc), L"DRGBPixelShader.hlsl", L"DRGBGeometryShader.hlsl"))
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
	return true;
}

void DeferredShader::setWorld(DirectX::XMMATRIX world)
{
	world = XMMatrixTranspose(world);
	this->worldCB.data.world = world;
	this->worldCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void DeferredShader::setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 camPos)
{
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);
	this->perFrameCB.data.view = view;
	this->perFrameCB.data.proj = proj;
	//need to set campos separately to enable check for backface culling
	//this->perFrameCB.data.camPos = camPos; 
	this->perFrameCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void DeferredShader::setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos)
{
	XMFLOAT4 cam = { campos.x, campos.y,campos.z,1.f };
	perFrameCB.data.camPos = cam;
}
void DeferredShader::setCBuffers()
{
	this->setConstanbuffer(GEOMETRY, 0, this->perFrameCB.getBuffer());
	this->setConstanbuffer(VERTEX, 0, this->perFrameCB.getBuffer());
	this->setConstanbuffer(PIXEL, 0, this->perFrameCB.getBuffer());
	this->setConstanbuffer(VERTEX, 1, this->worldCB.getBuffer());
	this->setConstanbuffer(GEOMETRY, 1, this->worldCB.getBuffer());
}

void DeferredShader::resetCB()
{
	this->setConstanbuffer(GEOMETRY, 0, nullptr);
	this->setConstanbuffer(VERTEX, 0, nullptr);
	this->setConstanbuffer(PIXEL, 0, nullptr);
	this->setConstanbuffer(VERTEX, 1, nullptr);
	this->setConstanbuffer(GEOMETRY, 1, nullptr);
}

void DeferredShader::prepGBuffer(float* color)
{
	this->gBuffer.setRenderTargets();
	this->gBuffer.clear(color);
}

void DeferredShader::prepForLight()
{
	this->gBuffer.setShaderResViews();
}
