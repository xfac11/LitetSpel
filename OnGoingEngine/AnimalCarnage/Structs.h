#pragma once
#include <DirectXMath.h>
struct Vertex3D
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT4 normal;
	/*DirectX::XMFLOAT4 tangent;
	DirectX::XMFLOAT4 biNormal;*/
};
struct WorldMatrix  //every model
{
	DirectX::XMMATRIX world;
};
struct PerFrameMatrices
{
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
	DirectX::XMFLOAT4 camPos;
};