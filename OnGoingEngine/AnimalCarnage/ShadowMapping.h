#pragma once
#include "Shader.h"
#include"Structs.h"
class ShadowMapping :
	public Shader
{
public:
	ShadowMapping();
	virtual~ShadowMapping();

	bool initialize();
	void setWorld(DirectX::XMMATRIX world);
	void setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 camPos);
	//uses the rendershader in Shader
	void setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos);
	void setCBuffers();

private:
	
	ID3D11BlendState* blendState;
	ID3D11RasterizerState* rasState;
	ConstantBuffer<WorldMatrix> worldCB;
	ConstantBuffer<PerFrameMatrices> perFrameCB;

	void shutdown();

};

