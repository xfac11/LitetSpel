#include "Shader.h"
#include"System.h"
Shader::Shader()
{
	this->geometryShader = nullptr;
	this->pixelShader = nullptr;
	this->vertexLayout = nullptr;
	this->vertexShader = NULL;
	this->comShader = nullptr;
}

Shader::~Shader()
{
	this->shutdown();
}

bool Shader::load(LPCWSTR  vs, D3D11_INPUT_ELEMENT_DESC  *inputDesc,UINT nrOfinput, LPCWSTR  ps, LPCWSTR  gs)
{
	ID3DBlob* errorBlob = nullptr;
	HRESULT result;
	if (vs != L"")
	{
		ID3DBlob* pVS = nullptr;

		result = D3DCompileFromFile(
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

		//int lSize = sizeof(inputDesc) / sizeof(inputDesc[0]);
		result = System::getDevice()->CreateInputLayout(inputDesc,nrOfinput, pVS->GetBufferPointer(), pVS->GetBufferSize(), &vertexLayout);

		if (FAILED(result))
		{
			return false;
		}
		// we do not need anymore this COM object, so we release it.
		pVS->Release();
	}
	if (ps != L"")
	{


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
	}
	if (gs != L"")
	{
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
	}

	



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

bool Shader::loadCS(LPCWSTR cs)
{
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pCS = nullptr;
	HRESULT result;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;
	result = D3DCompileFromFile(
		cs, // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"CS_main",		// entry point
		"cs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pCS,			// double pointer to ID3DBlob		
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
		if (pCS)
			pCS->Release();
		return false;
	}

	System::getDevice()->CreateComputeShader(
		pCS->GetBufferPointer(),
		pCS->GetBufferSize(),
		nullptr,
		&this->comShader
	);
	pCS->Release();
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

void Shader::shutdown()
{
	if(this->comShader!=nullptr)
		this->comShader->Release();

	if (this->vertexShader != nullptr)
		this->vertexShader->Release();

	if (this->pixelShader != nullptr)
		this->pixelShader->Release();

	if (this->geometryShader != nullptr)
		this->geometryShader->Release();

	if (this->sampler != nullptr)
		this->sampler->Release();

	if (this->vertexLayout != nullptr)
		this->vertexLayout->Release();
}

void Shader::renderShader(int vertexCount, int indexCount)
{
	//deviceContext->DrawIndexed(count, 0, 0);
	System::getDeviceContext()->DrawIndexed(indexCount, 0,0);
	//System::getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
}

void Shader::setShaders()
{
	System::getDeviceContext()->VSSetShader(this->vertexShader, nullptr, 0);
	System::getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->GSSetShader(this->geometryShader, nullptr, 0);
	System::getDeviceContext()->PSSetShader(this->pixelShader, nullptr, 0);
	System::getDeviceContext()->IASetInputLayout(this->vertexLayout);
}
