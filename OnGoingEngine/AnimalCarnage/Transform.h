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
protected:
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
	void setPosition(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setRotation(float x, float y, float z, float angle);

	~Transform();

	//Bullet
	void SetWorldToRigitBody();


};
//Bullet
btTransform XMMATRIX_to_btTransform(XMMATRIX const& mat);
XMMATRIX btTransform_to_XMMATRIX(btTransform const& trans);
//Get Position from btTransform
XMFLOAT3 btTransform_to_XMFLOAT3(btTransform const& trans);
