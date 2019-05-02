#include"Skybox.h"
#include"System.h"
Skybox::Skybox()
{
	/*Vertex cubeTemp[] = {
	-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,
	-SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,

	-SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,

	-SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,
	-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,

	-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,

	-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,

	-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,

	-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,

	-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,

	SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,

	SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE - SKYBOX_SIZE,SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE - SKYBOX_SIZE,SKYBOX_SIZE,

	-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,
	-SKYBOX_SIZE ,SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,

	-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE,
	-SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,SKYBOX_SIZE
	};*/
	Vertex cubeTemp[] =
	{
		-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		 SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		 SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		 SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		 SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE
	};

	DWORD index[] = {
		0, 1, 2, 2, 3, 0,
		4, 1, 0, 0, 5, 4,
		2, 6, 7, 7, 3, 2,
		4, 5, 7, 7, 6, 4,
		0, 3, 7, 7, 5, 0,
		1, 4, 2, 2, 4, 6
	};
	/*Vertex cubeTemp[] = {
		// Positions
		-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE, //1
		SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,

		-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,//2
		-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,

		SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,//3
		SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,

		-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,//4
		SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,

		-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,//5
		-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,

		-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,SKYBOX_SIZE,//6
		-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,SKYBOX_SIZE
	};*/
	//all the vertices inserted into cubetemp then into this->cube

	for (int i = 0; i < 8; i++)
	{
		this->vertices.push_back(cubeTemp[i]);
	}

	for (int i = 0; i < 36; i++)
	{
		this->indices.push_back(index[i]);
	}
	this->cubeSRV = nullptr;
	this->cubeTex = nullptr;
}

Skybox::~Skybox()
{
	this->shutDown();
}

void Skybox::shutDown()
{
	for (int i = 0; i < 6; i++)
	{
		this->textureLoad[i].Shutdown();
	}
	this->cubeSRV->Release();
	this->cubeTex->Release();
	//this->textureLoad->Shutdown();

	this->ps->Release();
	this->vs->Release();
	this->samplerState->Release();
	this->vertexLayout->Release();
}

bool Skybox::setWorld(DirectX::XMMATRIX world)
{
	world = XMMatrixTranspose(world);
	this->worldCB.data.world = world;
	this->worldCB.applyChanges(System::getDevice(), System::getDeviceContext());
	return true;
}

bool Skybox::setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
{
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);
	this->perFrameCB.data.view = view;
	this->perFrameCB.data.proj = proj;
	//need to set campos separately to enable check for backface culling
	//this->perFrameCB.data.camPos = camPos; 
	this->perFrameCB.applyChanges(System::getDevice(), System::getDeviceContext());
	return true;
}

void Skybox::setCB()
{
	System::getDeviceContext()->VSSetConstantBuffers(0, 1, this->perFrameCB.getAddressOfBuffer());
	System::getDeviceContext()->VSSetConstantBuffers(1, 1, this->worldCB.getAddressOfBuffer());
}

bool Skybox::initialize()
{
	if (FAILED(this->indexBuffer.initialize(this->indices.data(), this->indices.size(), System::getDevice())))
	{
		return false;
	}
	if(FAILED(this->vertexBuffer.initialize(this->vertices.data(), this->vertices.size(), System::getDevice())))
	{
		return false;
	}
	this->perFrameCB.initialize(System::getDevice());
	this->worldCB.initialize(System::getDevice());
	if (!this->initializeShaders())
	{
		return false;
	}
	return true;
}

bool Skybox::render()
{

	//Setvertexbandtexture
	UINT32 vertexSize = sizeof(Vertex);

	UINT32 offset = 0;
	System::getDeviceContext()->PSSetShaderResources(0, 1, &this->cubeSRV);
	System::getDeviceContext()->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), &vertexSize, &offset);
	System::getDeviceContext()->IASetIndexBuffer(this->indexBuffer.getBuffer(), DXGI_FORMAT_R32_UINT, 0);
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	System::getDeviceContext()->PSSetSamplers(0, 1, &this->samplerState);

	//render shader
	System::getDeviceContext()->VSSetShader(this->vs, nullptr, 0);
	System::getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->PSSetShader(this->ps, nullptr, 0);
	System::getDeviceContext()->IASetInputLayout(this->vertexLayout);

	System::getDeviceContext()->DrawIndexed(36, 0, 0);
	//36 for 6 faces with 6 vertecies for each face 6*6=36
	return true;
}

bool Skybox::setTexture(std::string file)
{
	std::string fileName = "Skyboxes/" + file + "_rt.tga";//right
	textureLoad[0].Initialize(fileName.c_str());
	fileName = "Skyboxes/" + file + "_lf.tga";//left
	textureLoad[1].Initialize(fileName.c_str());
	fileName = "Skyboxes/" + file + "_up.tga";//up
	textureLoad[2].Initialize(fileName.c_str());
	fileName = "Skyboxes/" + file + "_dn.tga";//down
	textureLoad[3].Initialize(fileName.c_str());
	fileName = "Skyboxes/" + file + "_bk.tga";//back
	textureLoad[4].Initialize(fileName.c_str());
	fileName = "Skyboxes/" + file + "_ft.tga";//forward
	textureLoad[5].Initialize(fileName.c_str());
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = textureLoad[0].getWidth();//Every texture has the same width and height
	texDesc.Height = textureLoad[0].getHeight();
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 6;//6 for all the faces of a cube
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.CPUAccessFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;//TextureCube
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = texDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = texDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	D3D11_SUBRESOURCE_DATA pData[6];
	for (int cubeMapFaceIndex = 0; cubeMapFaceIndex < 6; cubeMapFaceIndex++)
	{
		pData[cubeMapFaceIndex].pSysMem = this->textureLoad[cubeMapFaceIndex].getTextureCharArray();
		pData[cubeMapFaceIndex].SysMemPitch = this->textureLoad[cubeMapFaceIndex].getWidth() * 4;
		pData[cubeMapFaceIndex].SysMemSlicePitch = 0;
	}
	HRESULT hr = System::getDevice()->CreateTexture2D(&texDesc,
		pData, &cubeTex);
	if (FAILED(hr))
	{
		return false;
	}
	hr = System::getDevice()->CreateShaderResourceView(
		cubeTex, &SMViewDesc, &cubeSRV);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool Skybox::initializeShaders()
{
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"SkyVertexShader.hlsl", // filename vsFilename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pVS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			//OutputShaderErrorMessage(errorBlob, hwnd, vsFilename); //able when parameter active
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		else
		{
			//MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK); //able when parameter active
		}
		if (pVS)
			pVS->Release();
		return false;
	}

	System::getDevice()->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&vs
	);

	// create input layout (verified using vertex shader)
	// Press F1 in Visual Studio with the cursor over the datatype to jump
	// to the documentation online!
	// please read:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205117(v=vs.85).aspx
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32A32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			D3D11_APPEND_ALIGNED_ELEMENT, // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		}

	};
	result = System::getDevice()->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &vertexLayout);

	if (FAILED(result))
	{
		return false;
	}
	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"SkyPixelShader.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);


	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return false;
	}

	System::getDevice()->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &ps);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();



	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	// Create the texture sampler state.
	result = System::getDevice()->CreateSamplerState(&samplerDesc, &this->samplerState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


