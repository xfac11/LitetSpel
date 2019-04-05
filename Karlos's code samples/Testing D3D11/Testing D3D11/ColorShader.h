#ifndef COLORSHADER_H
#define COLORSHADER_H
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Structs.h"
#include <directxmath.h>
#include <fstream>
class ColorShader
{
private:
	ID3D11VertexShader* vertexShader; 
	ID3D11PixelShader* pixelShader; 
	ID3D11GeometryShader* geometryShader; 
	ID3D11InputLayout* vertexLayout; 
	ID3D11InputLayout* gridLayout;

	PerFrameMatrices* matricesPerFrame; //can be local in function setShaderParameter??
	ID3D11Buffer* MatrixPerFrameBuffer; 
	CBData* gConstantBufferData; //can be local in function setShaderParameter??
	ID3D11Buffer* ConstantBuffer;

	bool InitializeShader(ID3D11Device*& device, HWND hwnd, WCHAR* GS_name);//, WCHAR* vsFilename, WCHAR* psFileName); //WCHAR* gsFilename
	//bool InitializeGridShader(ID3D11Device*& device, HWND hwnd, WCHAR* GS_name);
															//void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage , HWND hwnd, WCHAR* shaderFilename);

	//bool SetShaderParameters(ID3D11DeviceContext*& deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);
	//void RenderShader(ID3D11DeviceContext* deviceContext, int count);
public:
	ColorShader();
	virtual~ColorShader();
	bool Initialize(ID3D11Device*device , HWND hwnd, WCHAR* GS_name);
	
	void Shutdown();
	//bool Render(ID3D11DeviceContext*& deviceContext, int count, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);
	bool SetShaderParameters(ID3D11DeviceContext*& deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);
	bool SetShaderGrid(ID3D11DeviceContext*& deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);
	void RenderShader(ID3D11DeviceContext* deviceContext, int count);
	//void RenderGrid(ID3D11DeviceContext* deviceContext, int count);

	void setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos);
};
#endif
