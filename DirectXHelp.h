#ifndef HELPER_H
#define HELPER_H
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <wrl/client.h>
#include "StateMachine.h"
#include "Lib/Shaders.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;//screen aspect

#define WIDTH  800
#define HEIGHT  600

struct LightPixelShader {
	float lightType;//4 nu 16
	XMFLOAT3 AmbientColor;
	float AmbientLight;
	XMFLOAT3 LightColor;
	float Strength;
	XMFLOAT3 position;
};
struct LIGHT {
	LightPixelShader Light[2];
};
#ifndef  gg_H
#define gg_H
struct PerFrameMatrices
{
	XMMATRIX view;
	XMMATRIX projection;
	XMFLOAT3 camPos;
	float padding;//4 nu 16
};
#endif // ! gg_H

struct Matrices {
	XMMATRIX world;
};
struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TextCoord;
	XMFLOAT3 Color;
};

struct GameData
{
	StateMachine machine;
	//Shader shader;
	//Camera cam;
};
typedef std::shared_ptr<GameData> GameDataRef;
#endif // !HELPER_H