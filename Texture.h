#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <WICTextureLoader.h>
#include <string>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

enum textureType {
	DIFFUSE, NORMAL,
	DEFAULT
};


class Texture
{
private:
	ComPtr<ID3D11ShaderResourceView> myTextureView = nullptr;
	textureType type = DIFFUSE;
	std::string filePath;
public:
	bool LoadTextureFromFile(ID3D11Device * device,std::string filePath, textureType type);
	bool LoadDefaultTexture(ID3D11Device * device,textureType type);
	int getType();
	ID3D11ShaderResourceView** getTextureView();
	~Texture();
};

