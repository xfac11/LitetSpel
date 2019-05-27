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
	btRigidBody * rigidbody;

	float getPitch(DirectX::XMVECTOR Quaternion);
	float getYaw(DirectX::XMVECTOR Quaternion);
	float getRoll(DirectX::XMVECTOR Quaternion);
protected:
	XMMATRIX rotationMatrix;
	XMFLOAT3 Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMFLOAT3 Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float Angle = 0.0f;
public:

	//vectors for bullet
	btVector3 LEFTVECTOR = btVector3(-1.0f, 0.0f, 0.0f);
	btVector3 RIGHTVECTOR = btVector3(1.0f, 0.0f, 0.0f);
	btVector3 UPVECTOR = btVector3(0.0f, 1.0f, 0.0f);
	btVector3 DOWNVECTOR = btVector3(0.0f, -1.0f, 0.0f);
	//

	const XMFLOAT3& getPosition()const;
	const XMFLOAT3& getScale()const;
	const XMFLOAT3& getRotation()const;
	XMMATRIX& getWorld() { return world; }
	btRigidBody* &getRigidbody() { return rigidbody; }
	void move(XMFLOAT3 Position);
	void move(float x, float y, float z);
	void setPosition(btVector3 xyz);
	void setPosition(XMFLOAT3 xyz);
	void setPosition(float x, float y, float z);
	void setScale(btVector3 xyz);
	void setScale(XMFLOAT3 xyz);
	void setScale(float x, float y, float z);
	void setRotation(float x, float y, float z, float angle);
	void setRotationRollPitchYaw(float x, float y, float z);
	void setMatrix(DirectX::XMMATRIX theMatrix);


	~Transform();

	//Bullet
	void SetWorldToRigitBody();
	void SetPosition(float x, float y, float z);
	XMFLOAT3 GetPosition();
	void Move(XMFLOAT3 posion);

};
//Bullet
btTransform XMMATRIX_to_btTransform(XMMATRIX const& mat);
XMMATRIX btTransform_to_XMMATRIX(btTransform const& trans);
//Get Position from btTransform
XMFLOAT3 btTransform_to_XMFLOAT3(btTransform const& trans);
XMFLOAT3 VECTORSUBTRACTION(const XMFLOAT3 other, const XMFLOAT3 other2);
XMFLOAT3 MULT(const XMFLOAT3 other, const XMFLOAT3 other2);
XMFLOAT3 MULT(const XMFLOAT3 other, const float other2);
XMFLOAT3 normalize(XMFLOAT3 pos);
float magnitude(XMFLOAT3 pos);