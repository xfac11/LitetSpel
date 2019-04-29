#pragma once
#include "Shader.h"
#include"Structs.h"
class LightShader :
	public Shader
{
public:
	LightShader();
	virtual~LightShader();
	bool initialize();
	void setWorld(DirectX::XMMATRIX world);
	void setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 camPos);
	//uses the rendershader in Shader
	void setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos);
	void setCBuffers();
	void renderShaderDir(int vertexCount, ID3D11DepthStencilView* view);
	void renderShaderPoint(int vertexCount, ID3D11DepthStencilView* view);
private:
	ConstantBuffer<WorldMatrix> worldCB;
	ConstantBuffer<PerFrameMatrices> perFrameCB;


	ID3D11SamplerState* sampler;
	ID3D11BlendState* blendState;

	ID3D11RasterizerState* rasState;
	ID3D11RasterizerState* fRasState;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* disDepthStencilState;
	ID3D11DepthStencilState* dpthQuad;


	void renderUnmark(int count);
	void renderPixels(int count);

	void shutdown();
};

