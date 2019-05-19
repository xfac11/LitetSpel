#pragma once
#include "Shader.h"
#include"Structs.h"
class VerticalBlur :
	public Shader
{
public:
	VerticalBlur();
	virtual ~VerticalBlur();
	bool initialize(int height, int width);
	void render(int vertexCount,ID3D11ShaderResourceView* shaderView);
	void setWorld(DirectX::XMMATRIX world);
	void setCBuffers();
	ID3D11ShaderResourceView*& getShaderView();
private:
	ID3D11ShaderResourceView* shaderView;
	ID3D11SamplerState* sampler;
	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* renderTarget;
	D3D11_VIEWPORT view;
	ConstantBuffer<WindowClient> windowCB;

};

