#include "SimpleShaderClass.h"
#include"System.h"

SimpleShader::SimpleShader()
{

}

SimpleShader::~SimpleShader()
{
	this->shutdown();
}

bool SimpleShader::initialize()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
	};

	UINT numElements = ARRAYSIZE(layout);
	std::wstring shaderfolder = L"..\\x64\\Debug\\";
	if (!this->load(L"SimpleVS.hlsl", layout, numElements, L"SimplePS.hlsl"))
		return false;

	HRESULT result;
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

void SimpleShader::setWorld(DirectX::XMMATRIX world)
{
	world = XMMatrixTranspose(world);
	this->worldCB.data.world = world;
	this->worldCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void SimpleShader::setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 camPos)
{
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);
	this->perFrameCB.data.view = view;
	this->perFrameCB.data.proj = proj;
	//need to set campos separately to enable check for backface culling
	//this->perFrameCB.data.camPos = camPos; 
	this->perFrameCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void SimpleShader::setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos)
{
	XMFLOAT4 cam = { campos.x, campos.y,campos.z,1.f };
	perFrameCB.data.camPos = cam;
}

void SimpleShader::setCBuffers()
{
	this->setConstanbuffer(VERTEX, 0, this->perFrameCB.getBuffer());
	this->setConstanbuffer(VERTEX, 1, this->worldCB.getBuffer());
}
void SimpleShader::renderShader(int vertexCount, int indexCount)
{
	System::getDeviceContext()->DrawIndexed(vertexCount, 0,0);
}
void SimpleShader::shutdown()
{
	if (this->blendState != nullptr)
		this->blendState->Release();
	if (this->rasState != nullptr)
		this->rasState->Release();
}
