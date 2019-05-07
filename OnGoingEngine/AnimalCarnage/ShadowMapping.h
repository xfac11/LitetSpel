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
	void setView(DirectX::XMMATRIX view);
	//uses the rendershader in Shader
	void setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos);
	void setCBuffers();
	void prepare();
	ID3D11ShaderResourceView*& getShadowMap();
private:
	
	ID3D11BlendState* blendState;
	ID3D11RasterizerState* rasState;
	ConstantBuffer<WorldMatrix> worldCB;
	ConstantBuffer<PerFrameMatrices> perFrameCB;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthTexture;
	ID3D11ShaderResourceView* depthShaderResource;

	void shutdown();

};

