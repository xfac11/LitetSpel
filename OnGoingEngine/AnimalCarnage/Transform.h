#pragma once
#include<DirectXMath.h>

using namespace DirectX;
class Transform
{
private:
	XMMATRIX world = XMMATRIX(XMMatrixIdentity());
	void ApplyTransform();
protected:
	XMFLOAT3 Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMFLOAT3 Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float Angle = 0.0f;

public:
	const XMFLOAT3 &getPosition()const;
	const XMFLOAT3 &getScale()const;
	const XMFLOAT3 &getRotation()const;

	XMMATRIX &getWorld() { return world; }

	void move(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setRotation(float x, float y, float z, float angle);

	~Transform();
};
