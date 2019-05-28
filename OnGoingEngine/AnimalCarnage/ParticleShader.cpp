#include "ParticleShader.h"
#include"System.h"

ParticleShader::ParticleShader() : Shader()
{
}

ParticleShader::~ParticleShader()
{
}

bool ParticleShader::initialize()
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
			"SIZE",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R16_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			D3D11_APPEND_ALIGNED_ELEMENT, // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		}
	};
	
	if (!this->load(L"ParticleVS.hlsl", inputDesc, ARRAYSIZE(inputDesc), L"ParticlePS.hlsl", L"ParticleGS.hlsl"))
		return false;

	if (FAILED(this->perFrameCB.initialize(System::getDevice())))
		return false;

	if (FAILED(this->cameraCB.initialize(System::getDevice())))
		return false;

	if (FAILED(this->particleConfigCB.initialize(System::getDevice())))
		return false;


	return true;
}

void ParticleShader::setSize(float size)
{
	this->particleConfigCB.data.size = size;
	this->particleConfigCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void ParticleShader::setWorld(const DirectX::XMMATRIX& world)
{
}

void ParticleShader::setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 campos)
{
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);
	this->perFrameCB.data.view = view;
	this->perFrameCB.data.proj = proj;
	this->perFrameCB.data.camPos = campos; 

	this->perFrameCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void ParticleShader::setCamera(DirectX::XMFLOAT3 camUp)
{
	this->cameraCB.data.up = camUp;
	this->cameraCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void ParticleShader::setCBuffers()
{
	this->setConstanbuffer(GEOMETRY, 0, this->perFrameCB.getBuffer());
	this->setConstanbuffer(GEOMETRY, 1, this->cameraCB.getBuffer());
	this->setConstanbuffer(GEOMETRY, 2, this->particleConfigCB.getBuffer());
}