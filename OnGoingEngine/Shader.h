#ifndef SHADER_H
#define SHADER_H
#include <d3d11.h>
#include <d3dcompiler.h>
#include"ConstantBuffer.h"
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
	bool load(char* vs, char* gs, char* ps, char* cs, D3D11_INPUT_ELEMENT_DESC* inputDesc);
	bool setConstanbuffer(ShaderType type, int index, ID3D11Buffer* buffer);

private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11GeometryShader* geometryShader;
	ID3D11ComputeShader* comShader;
	ID3D11InputLayout* vertexLayout;

};
#endif // !SHADER_H
