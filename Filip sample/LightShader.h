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
	
	void render(ID3D11DeviceContext* deviceContext, int count, ID3D11DepthStencilView* view);
	void RenderShader(ID3D11DeviceContext* deviceContext, int count);


	bool SetShaderParameters(ID3D11DeviceContext	*& deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, 
		  AnyLight light, DirectX::XMFLOAT3 camPos, int choice=0);

private:

	bool InitializeShader(ID3D11Device*& device, HWND hwnd);//, WCHAR* vsFilename, WCHAR* psFileName); //WCHAR* gsFilename
	
	void renderUnmark(ID3D11DeviceContext * deviceContext, int count);

	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11GeometryShader* geometryShader;
	ID3D11InputLayout* vertexLayout;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* disDepthStencilState;

	ID3D11RasterizerState* rasState;
	ID3D11RasterizerState* fRasState;
	ID3D11BlendState* blendState;

	ID3D11SamplerState* sampler;
	MatrixBuffers* data;
	AnyLight* dataSpec;
	ID3D11Buffer* MatrixPerFrameBuffer;
	ID3D11Buffer* LightPerFrameBuffer;
	CBData* gConstantBufferData; //can be local in function setShaderParameter??
	ID3D11Buffer* ConstantBuffer;

};
#endif // !LIGHTSHADER_H