#pragma once
#include "Shader.h"
#include"GBuffer.h"
#include"Structs.h"
class DeferredShader :
	public Shader
{
public:
	DeferredShader();
	~DeferredShader();
	bool initialize(int height, int width, float nearPlane, float farPlane);
	void setWorld(DirectX::XMMATRIX world);
	void setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 camPos);
	//uses the rendershader in Shader
	void setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos);
	GBuffer gBuffer;
private:
	ID3D11BlendState* blendState;
	ID3D11RasterizerState* rasState;
	ConstantBuffer<WorldMatrix> worldCB;
	ConstantBuffer<PerFrameMatrices> perFrameCB;

};

