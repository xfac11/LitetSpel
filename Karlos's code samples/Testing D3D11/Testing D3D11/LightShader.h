#ifndef LIGHTSHADER_H
#define LIGHTSHADER_H
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Structs.h"
#include <directxmath.h>
#include <fstream>
class LightShader
{
public:
	LightShader();
	~LightShader();
	bool Initialize(ID3D11Device*device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext*& deviceContext, int count, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);
	void RenderShader(ID3D11DeviceContext* deviceContext, int count);
	bool SetShaderParameters(ID3D11DeviceContext*& deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, 
		  PointLight light, DirectX::XMFLOAT3 camPos, int choice);

private:

	bool InitializeShader(ID3D11Device*& device, HWND hwnd);//, WCHAR* vsFilename, WCHAR* psFileName); //WCHAR* gsFilename
	
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11GeometryShader* geometryShader;
	ID3D11InputLayout* vertexLayout;

	ID3D11SamplerState* sampler;
	MatrixBuffers* data;
	PointLight* dataSpec;
	ID3D11Buffer* MatrixPerFrameBuffer;
	ID3D11Buffer* LightPerFrameBuffer;
	CBData* gConstantBufferData; //can be local in function setShaderParameter??
	ID3D11Buffer* ConstantBuffer;

};
#endif // !LIGHTSHADER_H