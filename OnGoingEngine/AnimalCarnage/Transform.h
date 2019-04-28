#pragma once
#include<DirectXMath.h>
#include "BulletSDK/include/btBulletDynamicsCommon.h"
#include "BulletSDK/include/btBulletCollisionCommon.h"
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

	//test
	XMFLOAT3 LeftVec = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	XMFLOAT3 RightVec = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3 UpVec = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 DownVec = XMFLOAT3(0.0f, -1.0f, 0.0f);
public:
	const XMFLOAT3& getPosition()const;
	const XMFLOAT3& getScale()const;
	const XMFLOAT3& getRotation()const;

	XMMATRIX& getWorld() { return world; }

	void move(XMFLOAT3 Position);
	void move(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setRotation(float x, float y, float z, float angle);

	~Transform();




};
//Bullet
btTransform XMMATRIX_to_btTransform(XMMATRIX const& mat);
XMMATRIX btTransform_to_XMMATRIX(btTransform const& trans);
//Get Position from btTransform
XMFLOAT3 btTransform_to_XMFLOAT3(btTransform const& trans);
