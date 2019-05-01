#ifndef Skybox_H
#define Skybox_H

#include"Texture.h"
#include"TextureLoad.h"
#include <d3d11.h>
#include <directxmath.h>
#include"Structs.h"
#include<vector>
#include"ConstantBuffer.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include <d3dcompiler.h>
#define SIZE 1.0f

class Skybox
{
public:
	Skybox();
	~Skybox();
	void shutDown();
	bool initialize();
	bool setTexture(std::string file);
	bool render();
	bool setWorld(DirectX::XMMATRIX world);
	bool setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
	void setCB();
private:
	struct Vertex
	{
		float x, y, z, w;//16 
	};
	struct Matrices
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};
	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;
	ConstantBuffer<Matrices> perFrameCB;
	ConstantBuffer<WorldMatrix> worldCB;
	ID3D11PixelShader *ps;
	ID3D11VertexShader *vs;
	ID3D11InputLayout *vertexLayout;
	ID3D11SamplerState* samplerState;
	ID3D11ShaderResourceView* cubeSRV;
	ID3D11Texture2D* cubeTex;
	TextureLoad textureLoad[6];
	std::vector<DWORD> indices;
	std::vector<Vertex> vertices;
	
	bool initializeShaders();
	bool initializeVertex();
	
};
#endif // !Skybox_H