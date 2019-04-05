#include"DeferedShader.h"

DeferedShader::DeferedShader()
{
	this->vertexShader = nullptr;
	this->pixelShader = nullptr;
	this->geometryShader = nullptr;
	this->vertexLayout = nullptr;
	this->MatrixPerFrameBuffer = nullptr;
}

DeferedShader::~DeferedShader()
{
}

bool DeferedShader::Initialize(ID3D11Device * device, HWND hwnd)
{
	bool result = false;

	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd);//, L"../Engine/color.vs", L"../Engine/color.ps");
	if (!result)
	{
		result = false;
	}
	else
		result = true;
	WVPdata = (MatrixBuffers*)_aligned_malloc(sizeof(MatrixBuffers), 16);
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(MatrixBuffers);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA pData;
	ZeroMemory(&pData, sizeof(pData));
	pData.pSysMem = WVPdata;
	pData.SysMemPitch = 0;
	pData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = device->CreateBuffer(&desc, &pData, &MatrixPerFrameBuffer);
	if (FAILED(hr))
	{
		// deal with error...
		result = false;
	}
	this->dataSpec = (Specular*)_aligned_malloc(sizeof(Specular), 16);
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(Specular);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&pData, sizeof(pData));
	pData.pSysMem = dataSpec;
	pData.SysMemPitch = 0;
	pData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&desc, &pData, &SpecPerFrameBuffer);
	if (FAILED(hr))
	{
		// deal with error...
		result = false;
	}


	return result;
}

void DeferedShader::Shutdown()
{

	// Release the vertex shader.
	if (vertexShader)
	{
		vertexShader->Release();
		vertexShader = 0;
	}

	// Release the pixel shader.
	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = 0;
	}
	if (this->geometryShader)
	{
		geometryShader->Release();
		geometryShader = 0;
	}

	// Release the layout.
	if (vertexLayout)
	{
		vertexLayout->Release();
		vertexLayout = 0;
	}
	if (this->MatrixPerFrameBuffer->Release())
	{
		this->MatrixPerFrameBuffer->Release();
	}	
	if (this->SpecPerFrameBuffer)
	{
		this->SpecPerFrameBuffer->Release();
	}
	if (this->WVPdata)
	{
		_aligned_free(WVPdata);
	}
	if (this->dataSpec)
	{
		_aligned_free(dataSpec);
	}
	if (this->gConstantBufferData)
	{
		_aligned_free(gConstantBufferData);
	}

	if (this->ConstantBuffer)
	{
		this->ConstantBuffer->Release();
	}

	if (this->sampler)
	{
		this->sampler->Release();
	}
}

//bool DeferedShader::Render(ID3D11DeviceContext *& deviceContext, int count, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
//{
//	return false;
//}

void DeferedShader::RenderShader(ID3D11DeviceContext * deviceContext, int count)
{


	deviceContext->VSSetShader(this->vertexShader, nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(this->geometryShader, nullptr, 0);
	deviceContext->PSSetShader(this->pixelShader, nullptr, 0);
	deviceContext->IASetInputLayout(this->vertexLayout);
	deviceContext->PSSetSamplers(0, 1, &this->sampler);
	//deviceContext->DrawIndexed(count, 0, 0);
	deviceContext->Draw(count, 0);
	//deviceContext->GSSetShader(nullptr, nullptr, 0);
}

bool DeferedShader::SetShaderParameters(ID3D11DeviceContext *& deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,
	DirectX::XMFLOAT3 specularAlbedo, float specularPower)
{

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	D3D11_MAPPED_SUBRESOURCE mappedMemorySpec;
	//	MatrixBufferType* dataPtr;
		//PerFrameMatrices* matricesPerFrame;
		//unsigned int bufferNumber;

		//Make sure to transpose matrices before sending them into the shader, this is a requirement for DirectX 11.

			// Transpose the matrices to prepare them for the shader.
	//worldMatrix = DirectX::XMMatrixIdentity();
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	DirectX::XMMATRIX WorldView = DirectX::XMMatrixMultiply(viewMatrix, worldMatrix);
	DirectX::XMMATRIX WorldViewProj = DirectX::XMMatrixMultiply(projectionMatrix, WorldView);
	WorldViewProj = DirectX::XMMatrixTranspose(WorldViewProj);
	this->WVPdata->world = worldMatrix;
	this->WVPdata->view = DirectX::XMMatrixTranspose(WorldView);
	this->WVPdata->projection = WorldViewProj;
	dataSpec->specularAlbedo = specularAlbedo;
	dataSpec->specularPower = specularPower;
	//Lock the m_matrixBuffer, set the new matrices inside it, and then unlock it.

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(MatrixPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	memcpy(mappedMemory.pData, this->WVPdata, sizeof(MatrixBuffers));

	// Unlock the constant buffer.
	deviceContext->Unmap(MatrixPerFrameBuffer, 0);

	result = deviceContext->Map(SpecPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemorySpec);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	memcpy(mappedMemorySpec.pData, dataSpec, sizeof(Specular));// use this and the flickering stops

	// Unlock the constant buffer.
	deviceContext->Unmap(SpecPerFrameBuffer, 0);

	//Now set the updated matrix buffer in the HLSL vertex shader.

		// Set the position of the constant buffer in the vertex shader.
	//bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	//deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	//deviceContext->PSSetConstantBuffers(0, 1, &ConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &SpecPerFrameBuffer);
	deviceContext->PSSetConstantBuffers(1, 1, &MatrixPerFrameBuffer);
	deviceContext->GSSetConstantBuffers(0, 1, &MatrixPerFrameBuffer);
	deviceContext->VSSetConstantBuffers(0, 1, &MatrixPerFrameBuffer); //could have bufferNumber = 0 dno why tho
	//deviceContext->GSSetConstantBuffers(0, 1, &MatrixPerFrameBuffer);


	return true;
}

void DeferedShader::setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos)
{
	WVPdata->camPos = campos;
}

bool DeferedShader::InitializeShader(ID3D11Device *& device, HWND hwnd)
{
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"DRGBVertexShader.hlsl", // filename vsFilename
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

	device->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&vertexShader
	);

	// create input layout (verified using vertex shader)
	// Press F1 in Visual Studio with the cursor over the datatype to jump
	// to the documentation online!
	// please read:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205117(v=vs.85).aspx
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
		//{
		//	"COLOR",
		//	0,				// same slot as previous (same vertexBuffer)
		//	DXGI_FORMAT_R32G32B32_FLOAT,
		//	0,
		//	D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
		//	D3D11_INPUT_PER_VERTEX_DATA,
		//	0
		//},
		{
			"NORMALPOS",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"THEPOINT",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"TANGENT", //normal maps
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,							
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"BINORMAL", //normal maps
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,							
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},

	};
	result = device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &vertexLayout);

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
		L"DRGBFragment.hlsl", // filename
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

	device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &pixelShader);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();

	ID3DBlob* pGS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;
	result = D3DCompileFromFile(
		L"DRGBGeometryShader1.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GSmain",		// entry point
		"gs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
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
		if (pGS)
			pGS->Release();
		return false;
	}

	device->CreateGeometryShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&geometryShader
	);
	pGS->Release();

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void DeferedShader::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	std::ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", LPCSTR(shaderFilename), MB_OK);

	//return;
}