#ifndef SHADER_H
#define SHADER_H
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include"ConstantBuffer.h"
#include<Windows.h>
enum ShaderType
{
	VERTEX,
	GEOMETRY,
	PIXEL,
	COMPUTE
};
class Shader
{
public:
	Shader();
	virtual~Shader();
	bool load(LPCWSTR  vs, D3D11_INPUT_ELEMENT_DESC  *inputDesc, UINT nrOfinput, LPCWSTR ps =L"", LPCWSTR  gs =L"");
	bool loadCS(LPCWSTR  cs);
	bool setConstanbuffer(ShaderType type, int index, ID3D11Buffer* buffer);
	virtual void renderShader(int vertexCount,int indexCount);
	virtual void setWorld(DirectX::XMMATRIX world)=0;
	virtual void setCBuffers()=0;
	void setShaders();
private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11GeometryShader* geometryShader;
	ID3D11ComputeShader* comShader;
	ID3D11InputLayout* vertexLayout;
	ID3D11SamplerState* sampler;

	void shutdown();
};
#endif // !SHADER_H
