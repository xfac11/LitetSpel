#pragma once
#include "Shader.h"
class Blur
{
public:
	Blur();
	~Blur();
private:
	ID3D11Texture2D *texTures[4];
	ID3D11RenderTargetView *renderTars[4];
	ID3D11ShaderResourceView *shaderResViews[4];
	D3D11_VIEWPORT view;
};

