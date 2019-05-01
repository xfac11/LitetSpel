#ifndef PHYSICS_H
#define PHYSICS_H

#include "btBulletDynamicsCommon.h"
#include <vector>
#include <iostream>
#include <string>
#include<DirectXMath.h>
#include <algorithm>

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
	void Update(float deltaTime);
	btRigidBody* addSphere(float radius, btVector3 Origin, float mass);
	btRigidBody* addBox(btVector3 Origin, btVector3 size, float mass);
	//void renderSphere(btRigidBody* sphere);
	//void renderPlane(btRigidBody* plane);
	//void renderBox(btRigidBody* box);
};

#endif // !PHYSICS_H
