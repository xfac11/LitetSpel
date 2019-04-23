#pragma once
#include <DirectXMath.h>
#include "Luna/include/Luna/Luna.h"
struct Vertex3D //must be same as Luna::Vertex
{
	DirectX::XMFLOAT3 position; 
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal; 
	DirectX::XMFLOAT3 tangent; 
	DirectX::XMFLOAT3 biNormal;

	void operator=(const Luna::Vertex& obj)
	{
		if (&obj != nullptr)
		{
			position = {obj.position[0],obj.position[1],obj.position[2]};
			uv = { obj.uv[0], obj.uv[1] };
			normal = { obj.normal[0],obj.normal[1], obj.normal[2] };
			tangent = { obj.tangent[0],obj.tangent[1],obj.tangent[2] };
			biNormal= { obj.bitangent[0], obj.bitangent[1],obj.bitangent[2] };
		}
		else
		{
			position = { 0,0,0 };
			uv = { 0,0};
			normal = { 0,0,0 };
			tangent = { 0,0,0 };
			biNormal = { 0,0,0 };
		}
	}
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