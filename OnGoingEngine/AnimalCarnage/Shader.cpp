#include "Shader.h"
#include"System.h"
Shader::Shader()
{
	this->geometryShader = nullptr;
	this->pixelShader = nullptr;
	this->vertexLayout = nullptr;
	this->vertexShader = nullptr;
	this->comShader = nullptr;
}

Shader::~Shader()
{
}

bool Shader::load(LPCWSTR  vs, LPCWSTR  gs, LPCWSTR  ps, LPCWSTR  cs, D3D11_INPUT_ELEMENT_DESC  inputDesc[])
{

	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		vs, // filename vsFilename
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
		&vertexShader
	);
	// create input layout (verified using vertex shader)
	// Press F1 in Visual Studio with the cursor over the datatype to jump
	// to the documentation online!
	// please read:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205117(v=vs.85).aspx
	//D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
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
	//		"TEXCOORD",
	//		0,
	//		DXGI_FORMAT_R32G32_FLOAT, //2 values
	//		0,
	//		D3D11_APPEND_ALIGNED_ELEMENT,
	//		D3D11_INPUT_PER_VERTEX_DATA,
	//		0
	//	},

	//	{
	//		"NORMALPOS",
	//		0,				// same slot as previous (same vertexBuffer)
	//		DXGI_FORMAT_R32G32B32_FLOAT,
	//		0,
	//		D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
	//		D3D11_INPUT_PER_VERTEX_DATA,
	//		0
	//	},
	//	{
	//		"THEPOINT",
	//		0,				// same slot as previous (same vertexBuffer)
	//		DXGI_FORMAT_R32G32B32_FLOAT,
	//		0,
	//		D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
	//		D3D11_INPUT_PER_VERTEX_DATA,
	//		0
	//	},
	//	{
	//		"TANGENT", //normal maps
	//		0,
	//		DXGI_FORMAT_R32G32B32A32_FLOAT,
	//		0,
	//		D3D11_APPEND_ALIGNED_ELEMENT,
	//		D3D11_INPUT_PER_VERTEX_DATA,
	//		0
	//	},
	//	{
	//		"BINORMAL", //normal maps
	//		0,
	//		DXGI_FORMAT_R32G32B32A32_FLOAT,
	//		0,
	//		D3D11_APPEND_ALIGNED_ELEMENT,
	//		D3D11_INPUT_PER_VERTEX_DATA,
	//		0
	//	}

	//};
	result = System::getDevice()->CreateInputLayout(inputDesc, _ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &vertexLayout);

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
		ps, // filename
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

	System::getDevice()->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &pixelShader);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();

	ID3DBlob* pGS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;
	result = D3DCompileFromFile(
		gs, // filename
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
		return false;
	}

	System::getDevice()->CreateGeometryShader(
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
	result = System::getDevice()->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(result))
	{
		return false;
	}

	return true;

}

bool Shader::setConstanbuffer(ShaderType type, int index, ID3D11Buffer * buffer)
{
	switch (type)
	{
	case VERTEX:
		System::getDeviceContext()->VSSetConstantBuffers(index, 1, &buffer);
		break;
	case GEOMETRY:
		System::getDeviceContext()->GSSetConstantBuffers(index, 1, &buffer);
		break;
	case PIXEL:
		System::getDeviceContext()->PSSetConstantBuffers(index, 1, &buffer);
		break;
	case COMPUTE:
		System::getDeviceContext()->CSSetConstantBuffers(index, 1, &buffer);
		break;
	}
	return true;
}

void Shader::renderShader(int vertexCount, int indexCount)
{
	System::getDeviceContext()->VSSetShader(this->vertexShader, nullptr, 0);
	System::getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->GSSetShader(this->geometryShader, nullptr, 0);
	System::getDeviceContext()->PSSetShader(this->pixelShader, nullptr, 0);
	System::getDeviceContext()->IASetInputLayout(this->vertexLayout);

	//deviceContext->DrawIndexed(count, 0, 0);
	System::getDeviceContext()->DrawIndexed(indexCount, 0,0);
	System::getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
}
