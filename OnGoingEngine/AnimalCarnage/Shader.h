#ifndef SHADER_H
#define SHADER_H
#include <d3d11.h>
#include <d3dcompiler.h>
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
	~Shader();
	bool load(LPCWSTR  vs, LPCWSTR  gs, LPCWSTR  ps, LPCWSTR  cs, D3D11_INPUT_ELEMENT_DESC  inputDesc[]);
	bool setConstanbuffer(ShaderType type, int index, ID3D11Buffer* buffer);
	virtual void renderShader(int vertexCount,int indexCount);
private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11GeometryShader* geometryShader;
	ID3D11ComputeShader* comShader;
	ID3D11InputLayout* vertexLayout;
	ID3D11SamplerState* sampler;
};
#endif // !SHADER_H
