#include "ColorShader.h"

bool ColorShader::InitializeShader(ID3D11Device*& device, HWND hwnd, WCHAR* GS_name)//, WCHAR* vsFilename, WCHAR* psFileName) //WCHAR* gsFilename
{
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"Vertex.hlsl", // filename vsFilename
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
		return result;
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
			"NORMAL",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"MYVALUE",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}

	};

	device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &vertexLayout);
	
	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"Fragment.hlsl", // filename
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
		return result;
	}

	device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &pixelShader);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();

	ID3DBlob* pGS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;
	result = D3DCompileFromFile(
		GS_name, // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
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
		return result;
	}

	device->CreateGeometryShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&geometryShader
	);
	pGS->Release();
	return true;
}

//bool ColorShader::InitializeGridShader(ID3D11Device *& device, HWND hwnd)
//{
//	ID3DBlob* pVS = nullptr;
//	ID3DBlob* errorBlob = nullptr;
//
//	HRESULT result = D3DCompileFromFile(
//		L"VertexGrid.hlsl", // filename vsFilename
//		nullptr,		// optional macros
//		nullptr,		// optional include files
//		"VS_main",		// entry point
//		"vs_5_0",		// shader model (target)
//		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
//		0,				// IGNORE...DEPRECATED.
//		&pVS,			// double pointer to ID3DBlob		
//		&errorBlob		// pointer for Error Blob messages.
//	);
//
//	if (FAILED(result))
//	{
//		if (errorBlob)
//		{
//			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
//
//			errorBlob->Release();
//		}
//		if (pVS)
//			pVS->Release();
//		return result;
//	}
//	device->CreateVertexShader(pVS->GetBufferPointer(),pVS->GetBufferSize(),nullptr,&vertexShader);
//	D3D11_INPUT_ELEMENT_DESC inputGridDesc[] = {
//	{
//		"SV_POSITION",		// "semantic" name in shader
//		0,				// "semantic" index (not used)
//		DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
//		0,							 // input slot
//		D3D11_APPEND_ALIGNED_ELEMENT, // offset of first element
//		D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
//		0							 // used for INSTANCING (ignore)
//	},
//	{
//		"COLOR",
//		0,				// same slot as previous (same vertexBuffer)
//		DXGI_FORMAT_R32G32B32_FLOAT,
//		0,
//		D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
//		D3D11_INPUT_PER_VERTEX_DATA,
//		0
//	}
//	};
//	device->CreateInputLayout(inputGridDesc, ARRAYSIZE(inputGridDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gridLayout);
//
//	pVS->Release();
//
//	ID3DBlob* pPS = nullptr;
//	if (errorBlob) errorBlob->Release();
//	errorBlob = nullptr;
//
//	result = D3DCompileFromFile(
//		L"FragmentGrid.hlsl", // filename
//		nullptr,		// optional macros
//		nullptr,		// optional include files
//		"PS_main",		// entry point
//		"ps_5_0",		// shader model (target)
//		D3DCOMPILE_DEBUG,	// shader compile options
//		0,				// effect compile options
//		&pPS,			// double pointer to ID3DBlob		
//		&errorBlob			// pointer for Error Blob messages.
//	);
//
//	if (FAILED(result))
//	{
//		if (errorBlob)
//		{
//			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
//			// release "reference" to errorBlob interface object
//			errorBlob->Release();
//		}
//		if (pPS)
//			pPS->Release();
//		return result;
//	}
//	device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &pixelShader);
//	pPS->Release();
//
//	ID3DBlob* pGS = nullptr;
//	if (errorBlob) errorBlob->Release();
//	errorBlob = nullptr;
//	result = D3DCompileFromFile(
//		L"GeometryGrid.hlsl", // filename
//		nullptr,		// optional macros
//		nullptr,		// optional include files
//		"GS_main",		// entry point
//		"gs_5_0",		// shader model (target)
//		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
//		0,				// IGNORE...DEPRECATED.
//		&pGS,			// double pointer to ID3DBlob		
//		&errorBlob		// pointer for Error Blob messages.
//	);
//
//	// compilation failed?
//	if (FAILED(result))
//	{
//		if (errorBlob)
//		{
//			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
//			// release "reference" to errorBlob interface object
//			errorBlob->Release();
//		}
//		if (pGS)
//			pGS->Release();
//		return result;
//	}
//
//	device->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &geometryShader);
//	pGS->Release();
//	return true;
//}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
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

bool ColorShader::SetShaderParameters(ID3D11DeviceContext *& deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	//Make sure to transpose matrices before sending them into the shader, this is a requirement for DirectX 11.

	// Transpose the matrices to prepare them for the shader.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	DirectX::XMMATRIX WorldView = DirectX::XMMatrixMultiply(viewMatrix, worldMatrix);
	DirectX::XMMATRIX WorldViewProj = DirectX::XMMatrixMultiply(projectionMatrix, WorldView);
	WorldViewProj = DirectX::XMMatrixTranspose(WorldViewProj);
	matricesPerFrame->worldMatrix = worldMatrix;
	matricesPerFrame->WorldViewProj = WorldViewProj;
	
	//Lock the m_matrixBuffer, set the new matrices inside it, and then unlock it.

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(MatrixPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	memcpy(mappedMemory.pData, matricesPerFrame, sizeof(PerFrameMatrices));

	// Unlock the constant buffer.
	deviceContext->Unmap(MatrixPerFrameBuffer, 0);

	//Now set the updated matrix buffer in the HLSL vertex shader.


	// Finanly set the constant buffer in the vertex shader with the updated values.
	//deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &ConstantBuffer);
	deviceContext->PSSetConstantBuffers(1, 1, &MatrixPerFrameBuffer);
	deviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer); 
	deviceContext->GSSetConstantBuffers(0, 1, &MatrixPerFrameBuffer);

	return true;
}

//bool ColorShader::SetShaderGrid(ID3D11DeviceContext *& deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
//{
//	HRESULT result;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	//MatrixBufferType* dataPtr;
//	//unsigned int bufferNumber;
//
//	// Transpose the matrices to prepare them for the shader.
//	worldMatrix = XMMatrixTranspose(worldMatrix);
//	viewMatrix = XMMatrixTranspose(viewMatrix);
//	projectionMatrix = XMMatrixTranspose(projectionMatrix);
//	DirectX::XMMATRIX WorldView = DirectX::XMMatrixMultiply(viewMatrix, worldMatrix);
//	DirectX::XMMATRIX WorldViewProj = DirectX::XMMatrixMultiply(projectionMatrix, WorldView);
//	WorldViewProj = DirectX::XMMatrixTranspose(WorldViewProj);
//	matricesPerFrame->worldMatrix = worldMatrix;
//	matricesPerFrame->WorldViewProj = WorldViewProj;
//
//
//	result = deviceContext->Map(MatrixPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	memcpy(mappedResource.pData, matricesPerFrame, sizeof(PerFrameMatrices));
//	deviceContext->Unmap(MatrixPerFrameBuffer, 0);
//	deviceContext->PSSetConstantBuffers(0, 1, &ConstantBuffer); //added
//	deviceContext->PSSetConstantBuffers(1, 1, &MatrixPerFrameBuffer);  //added
//	deviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer); 
//	deviceContext->GSSetConstantBuffers(0, 1, &MatrixPerFrameBuffer);  //added
//
//	return true;
//}

void ColorShader::RenderShader(ID3D11DeviceContext * deviceContext, int count)
{
	deviceContext->VSSetShader(this->vertexShader, nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(this->geometryShader, nullptr, 0);
	deviceContext->PSSetShader(this->pixelShader, nullptr, 0);
	deviceContext->IASetInputLayout(this->vertexLayout);

	//deviceContext->DrawIndexed(count, 0, 0);
	deviceContext->Draw(count, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
}

//void ColorShader::RenderGrid(ID3D11DeviceContext * deviceContext, int count)
//{
//	// Set the vertex input layout.
//	deviceContext->IASetInputLayout(this->gridLayout);
//
//	// Set the vertex and pixel shaders that will be used to do the rendering.
//	deviceContext->VSSetShader(this->vertexShader, NULL, 0);
//	deviceContext->PSSetShader(this->pixelShader, NULL, 0);
//
//	// Render the data.
//	deviceContext->DrawIndexed(count, 0, 0);
//}

void ColorShader::setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos)
{
	matricesPerFrame->camPos = campos;
}


ColorShader::ColorShader()
{
	this->vertexShader = nullptr;
	this->pixelShader = nullptr;
	this->geometryShader = nullptr;
	this->vertexLayout = nullptr;
	this->gridLayout = nullptr;
	//PerFrameMatrices* gMatricesPerFrame;
	//this->matrixBuffer = nullptr;
}

ColorShader::~ColorShader()
{
}

bool ColorShader::Initialize(ID3D11Device *device, HWND hwnd,WCHAR* GS_name)
{
	bool result = false;
	gConstantBufferData = (CBData*)_aligned_malloc(sizeof(CBData), 16);
	gConstantBufferData->colour[0] = 0.5f;
	gConstantBufferData->colour[1] = 0.5f;
	gConstantBufferData->colour[2] = 0.5f;
	gConstantBufferData->colour[3] = 1.0f;
	gConstantBufferData->offset = 0.0f;



	// Initialize the vertex and pixel shaders.
	//if (id == 0)
	//{

	result = InitializeShader(device, hwnd, GS_name);//, L"../Engine/color.vs", L"../Engine/color.ps");
	if (!result)
	{
		result = false;
	}
	else
		result = true;
	//}
	//else if (id == 1)
	//{

	//	result = InitializeGridShader(device, hwnd);//, L"../Engine/color.vs", L"../Engine/color.ps");
	//	if (!result)
	//	{
	//		result = false;
	//	}
	//	else
	//		result = true;
	//}

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(CBData);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA pData;
	ZeroMemory(&pData, sizeof(pData));
	pData.pSysMem = gConstantBufferData;
	pData.SysMemPitch = 0;
	pData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = device->CreateBuffer(&desc, &pData, &ConstantBuffer);
	if (FAILED(hr))
	{
		// deal with error...
		result = false;
	}

	matricesPerFrame = (PerFrameMatrices*)_aligned_malloc(sizeof(PerFrameMatrices), 16);
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(PerFrameMatrices);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&pData, sizeof(pData));
	pData.pSysMem = gConstantBufferData;
	pData.SysMemPitch = 0;
	pData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&desc, &pData, &MatrixPerFrameBuffer);
	if (FAILED(hr))
	{
		// deal with error...
		result = false;
	}

	return result;
}

void ColorShader::Shutdown()
{
	if (matricesPerFrame)
	{
		_aligned_free(matricesPerFrame);
	}
	if (MatrixPerFrameBuffer)
	{
		MatrixPerFrameBuffer->Release();
		MatrixPerFrameBuffer = nullptr;
	}
	if (gConstantBufferData)
	{
		_aligned_free(gConstantBufferData); //struct
	}
	if (ConstantBuffer)
	{
		ConstantBuffer->Release();
		ConstantBuffer = nullptr;
	}
	// Release the layout.
	if (vertexLayout)
	{
		vertexLayout->Release();
		vertexLayout = nullptr;
	}

	// Release the pixel shader.
	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = nullptr;
	}

	// Release the vertex shader.
	if (vertexShader)
	{
		vertexShader->Release();
		vertexShader = nullptr;
	}
	if (geometryShader)
	{
		geometryShader->Release();
		geometryShader = nullptr;
	}
}

//bool ColorShader::Render(ID3D11DeviceContext*& deviceContext, int count, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
//{
//	bool result = false;
//
//	// Set the shader parameters that it will use for rendering.
//	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
//	if (!result)
//	{
//		result = false;
//	}
//	else
//	{
//		// Now render the prepared buffers with the shader.
//		RenderShader(deviceContext, count);
//	}
//	return result;
//}