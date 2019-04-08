#ifndef LIGHT_H
#define LIGHT_H
#include <d3d11.h>
#include <directxmath.h>
#include <math.h>
#include<iostream>
#include"Model.h"
#include"LightShader.h"
#include<new>
enum LightTypes
{
	POINTLIGHT,
	DIRECTIONALLIGHT,
	SPOTLIGHT
};
class Light
{
private:
	
	AnyLight* light;
	Model modelsVolumeLights[3];//POINTLIGHT: sphere  DIRECTIONALLIGHT: quad  SPOTLIGHT: cone
	int cap;
	int nrOfLights;
public:
	Light();
	~Light();
	void shutdown();
	void initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	float calcRadius();
	void addLight(LightTypes type, DirectX::XMFLOAT4 color, DirectX::XMFLOAT4 position,DirectX::XMFLOAT4 direction);
	void drawLights(LightShader * shader, ID3D11DepthStencilView* view, ID3D11Device * device, ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX proj, DirectX::XMFLOAT3 camPos, int choice);
	void moveLight(int index, float x, float y, float z);
	void setPosLight(int index, float x, float y, float z);
};
#endif // !LIGHT_H
