#include "Physics.h"

Physics::Physics() :broadphase(new btDbvtBroadphase()),
collisionConfig(new btDefaultCollisionConfiguration()),
solver(new btSequentialImpulseConstraintSolver)
{
	
	dispatcher = new btCollisionDispatcher(collisionConfig);
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	//ska vara -10 av någon anledning
	this->world->setGravity(btVector3(0, -9.82, 0));
	//temp plane inf
	//btTransform t;
	//t.setIdentity();
	//t.setOrigin(btVector3(0, 0, 0));
	////set shape for object
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	//set motionshape aka set postion
	btMotionState* motion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	//body definition check doc
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);

	btRigidBody* body = new btRigidBody(info);

	world->addRigidBody(body);
	bodies.push_back(body);

}


Physics::~Physics()
{

	//osäker på om man deletar på detta sätt
	for (int i = 0; i < bodies.size(); i++)
	{
		this->world->removeRigidBody(bodies[i]);
		btMotionState* motionState = bodies[i]->getMotionState();
		btCollisionShape* shape = bodies[i]->getCollisionShape();
		delete bodies[i];
		delete shape;
		delete motionState;
	}
	delete dispatcher;
	delete collisionConfig;
	delete solver;
	delete broadphase;
	delete world;
	bodies.clear();

}

void Physics::Update()
{
	for (int i = 0; i < this->bodies.size(); i++)
	{
		renderSphere(bodies[i]);
		renderPlane(bodies[i]);
	}

	this->world->stepSimulation(1 / 60.f, 10);
}

btRigidBody* Physics::addSphere(float rad, float x, float y, float z, float mass)
{	//add object set transform
	btTransform t; //
	t.setIdentity();
	t.setOrigin(btVector3(x, y,z));
	btSphereShape * sphere = new btSphereShape(rad); //raduius

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0f) {
		sphere->calculateLocalInertia(mass, inertia);
	}

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);

	btRigidBody* body = new btRigidBody(info);

	this->world->addRigidBody(body);
	bodies.push_back(body);
	return body;
}

void Physics::renderSphere(btRigidBody* sphere)
{
	if (sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE) {
		return;
	}
	float r = ((btSphereShape*)sphere->getCollisionShape())->getRadius();
	btTransform t;
	sphere->getMotionState()->getWorldTransform(t);
}

void Physics::renderPlane(btRigidBody* plane)
{
	if (plane->getCollisionShape()->getShapeType() != STATIC_PLANE_PROXYTYPE) {
		return;
	}
	btTransform t;
	plane->getMotionState()->getWorldTransform(t);
}
