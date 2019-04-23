/*
Move Everything here to Luna.h???
use #include "Luna/Luna.h"
Remove this class
*/


#pragma once
#include <DirectXMath.h>
struct Vertex3D //must be same as Luna::Vertex
{
	DirectX::XMFLOAT3 position; //float 3
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal; //float 3
	DirectX::XMFLOAT3 tangent; //float3
	DirectX::XMFLOAT3 biNormal;  //float3
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