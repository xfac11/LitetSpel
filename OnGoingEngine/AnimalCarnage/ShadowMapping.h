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
	void setWorld(const DirectX::XMMATRIX& world);
	void setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 camPos);
	void setView(DirectX::XMMATRIX view);
	//uses the rendershader in Shader
	void setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos);
	void setCBuffers();
	void prepare(DirectX::XMMATRIX& view);

	void setPSDepthView();
	ID3D11ShaderResourceView*& getShadowMap();
	void setSampler();
private:
	
	ID3D11BlendState* blendState;
	ID3D11RasterizerState* rasState;
	ConstantBuffer<WorldMatrix> worldCB;
	ConstantBuffer<PerFrameMatrices> perFrameCB;

	D3D11_VIEWPORT vp;
	ID3D11SamplerState* sampler;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthTexture;
	ID3D11ShaderResourceView* depthShaderResource;

	void shutdown();

};

