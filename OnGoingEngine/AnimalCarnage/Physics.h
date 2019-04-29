#ifndef PHYSICS_H
#define PHYSICS_H

#include "btBulletDynamicsCommon.h"
#include <vector>
#include <iostream>
#include <string>
#include<DirectXMath.h>
#include <algorithm>
class Transformbt
{
public:
	Transformbt(float locX, float locY, float locZ, float quatX, float quatY, float quatZ, float quatW);
	float locX;
	float locY;
	float locZ;
	float quatX;
	float quatY;
	float quatZ;
	float quatW;
};
class Physics
{
private:
	btDynamicsWorld* world;
	btDispatcher* dispatcher;
	btBroadphaseInterface* broadphase;
	btConstraintSolver* solver;
	btCollisionConfiguration* collisionConfig;
	std::vector<btRigidBody*> bodies;
	std::vector<btCollisionShape*> shapes;

public:
	Physics();
	~Physics();
	void Update();
	btRigidBody* addSphere(float rad, float x, float y, float z, float mass);
	void renderSphere(btRigidBody* sphere);
	void renderPlane(btRigidBody* plane);

};

#endif // !PHYSICS_H
