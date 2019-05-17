#include "AnimationShader.h"
#include"System.h"
void AnimationShader::shutdown()
{
	if (this->blendState != nullptr)
		this->blendState->Release();
	if (this->rasState != nullptr)
		this->rasState->Release();
}
AnimationShader::AnimationShader()
{
	this->blendState = nullptr;
	this->rasState = nullptr;
}
AnimationShader::~AnimationShader()
{
	this->shutdown();
}

bool AnimationShader::initialize()
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
			"JOINTS",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
			//for normal mapping. tangent binormal
		 },
		{
			"WEIGHTS",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
			//for normal mapping. tangent binormal
		 }
	};
	HRESULT result;
	if (!this->load(L"animatedEntityVertex.hlsl", inputDesc, ARRAYSIZE(inputDesc)))
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
	result = this->jointCB.initialize(System::getDevice());
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void AnimationShader::setWorld(DirectX::XMMATRIX world)
{
	world = XMMatrixTranspose(world);
	this->worldCB.data.world = world;
	this->worldCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void AnimationShader::setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 camPos)
{
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);
	this->perFrameCB.data.view = view;
	this->perFrameCB.data.proj = proj;
	//need to set campos separately to enable check for backface culling
	this->perFrameCB.data.camPos = camPos; 
	this->perFrameCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void AnimationShader::setJointData(std::vector<KeyFrame> keyframes)
{
	for (int i = 0; i < keyframes.size(); i++)
	{
		this->jointCB.data.jointTransformations[i] = keyframes[i].getJointKeyFrames().getLocalTransform();

	}
	
	this->jointCB.applyChanges(System::getDevice(), System::getDeviceContext());
}

void AnimationShader::setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos)
{
	XMFLOAT4 cam = { campos.x, campos.y,campos.z,1.f };
	perFrameCB.data.camPos = cam;
}

void AnimationShader::setCBuffers()
{
	this->setConstanbuffer(GEOMETRY, 0, this->perFrameCB.getBuffer());
	this->setConstanbuffer(VERTEX, 0, this->perFrameCB.getBuffer());
	this->setConstanbuffer(PIXEL, 0, this->perFrameCB.getBuffer());
	this->setConstanbuffer(VERTEX, 1, this->worldCB.getBuffer());
	this->setConstanbuffer(GEOMETRY, 1, this->worldCB.getBuffer());
	if(this->jointCB.getAddressOfBuffer()!=nullptr)
		this->setConstanbuffer(VERTEX, 2, this->jointCB.getBuffer());
}
