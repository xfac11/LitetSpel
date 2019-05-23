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
	DirectX::XMINT4 Joint;
	//DirectX::XMFLOAT4 Joint;
	DirectX::XMFLOAT4 Weights;

	void operator=(const Luna::Vertex& obj)
	{
		if (&obj != nullptr)
		{
			position = {obj.position[0],obj.position[1],obj.position[2]};
			uv = { obj.uv[0], obj.uv[1] };
			normal = { obj.normal[0],obj.normal[1], obj.normal[2] };
			tangent = { obj.tangent[0],obj.tangent[1],obj.tangent[2] };
			biNormal= { obj.bitangent[0], obj.bitangent[1],obj.bitangent[2] };
			Joint = { 0,0,0,0 };
			Weights = { 0.f,0.f,0.f,0.f };
		}
		else
		{
			position = { 0.f,0.f,0.f };
			uv = { 0.f,0.f};
			normal = { 0.f,0.f,0.f };
			tangent = { 0.f,0.f,0.f };
			biNormal = { 0.f,0.f,0.f };
			Joint = { 0,0,0,0 };
			Weights = { 0.f,0.f,0.f,0.f };
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

struct skinningData
{
	bool hasSkeleton = false;
	DirectX::XMMATRIX  jointTransformations[80];
};
struct WindowClient
{
	int height;
	int width;

};
struct ParticleCamera
{
	DirectX::XMFLOAT3 up;
};