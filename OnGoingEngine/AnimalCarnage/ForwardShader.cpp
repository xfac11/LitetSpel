#include"ForwardShader.h"
#include"System.h"
ForwardShader::ForwardShader()
	:Shader()
{
	this->blendState = nullptr;
	this->rasState = nullptr;
}

ForwardShader::~ForwardShader()
{
	this->shutdown();
}

bool ForwardShader::initialize()
{

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"SV_POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32A32_FLOAT, // size of ONE element (3 floats)
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
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
			}
		 //for normal mapping. tangent binormal

		//},
		//{
		//	"TANGENT", //normal maps
		//	0,
		//	DXGI_FORMAT_R32G32B32A32_FLOAT,
		//	0,
		//	D3D11_APPEND_ALIGNED_ELEMENT,
		//	D3D11_INPUT_PER_VERTEX_DATA,
		//	0
		//},
		//{
		//	"BINORMAL", //normal maps
		//	0,
		//	DXGI_FORMAT_R32G32B32A32_FLOAT,
		//	0,
		//	D3D11_APPEND_ALIGNED_ELEMENT,
		//	D3D11_INPUT_PER_VERTEX_DATA,
		//	0
		//} 


	};
	HRESULT result;
	if (!this->load(L"VertexShader.hlsl", inputDesc,ARRAYSIZE(inputDesc), L"PixelShader.hlsl", L"GeometryShader.hlsl"))
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

	//Rasteriser state, blend state, depthstencil state is in GraphicsDevice. 
	//Like a default. The rasteriser state is set in the initialize of GraphicsDevice
}

void ForwardShader::setWorld(DirectX::XMMATRIX world)
{
	world = XMMatrixTranspose(world);
	this->worldCB.data.world = world;
	this->worldCB.applyChanges(System::getDevice(), System::getDeviceContext());
	this->setConstanbuffer(VERTEX, 1, this->worldCB.getBuffer());
	this->setConstanbuffer(GEOMETRY, 1, this->worldCB.getBuffer());
}

void ForwardShader::setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj,DirectX::XMFLOAT4 camPos)
{
	
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);
	/*DirectX::XMMATRIX WorldView = DirectX::XMMatrixMultiply(viewMatrix, worldMatrix);
	DirectX::XMMATRIX WorldViewProj = DirectX::XMMatrixMultiply(projectionMatrix, WorldView);*/
	//WorldViewProj = DirectX::XMMatrixTranspose(WorldViewProj);
	//data->world = worldMatrix;
	//data->view = XMMatrixTranspose(WorldView);
	//data->projection = WorldViewProj;
	this->perFrameCB.data.view = view;
	this->perFrameCB.data.proj = proj;
	this->perFrameCB.data.camPos = camPos;
	this->perFrameCB.applyChanges(System::getDevice(),System::getDeviceContext());
	//System::getDeviceContext()->GSSetConstantBuffers(0, 1, this->perFrameCB.getAddressOfBuffer());
	this->setConstanbuffer(GEOMETRY, 0, this->perFrameCB.getBuffer());
	this->setConstanbuffer(VERTEX, 0, this->perFrameCB.getBuffer());
	this->setConstanbuffer(PIXEL, 0, this->perFrameCB.getBuffer());
}

void ForwardShader::shutdown()
{
	if(this->blendState!=nullptr)
		this->blendState->Release();
	if (this->rasState != nullptr)
		this->rasState->Release();
}
