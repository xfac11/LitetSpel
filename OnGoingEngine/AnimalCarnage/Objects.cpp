#include "Objects.h"
#include "System.h"
#include <string>
#include <random>
#include <iostream>
void Objects::SimplePlatformMovement(float dt)
{
	XMFLOAT3 current = ObjectOBJ->getPosition();

	if (move == false) {
	
		if (this->ObjectOBJ->getPosition().x > position2.x) {
			move = true;

		}
		else {
			XMFLOAT3 firstVector = MULT(current, firstriktningsVector);
			XMFLOAT3 Vector = MULT(firstVector, dt/5);

			ObjectOBJ->getRigidbody()->setLinearVelocity(btVector3(Vector.x * 50, Vector.y * 50,0));
		}
	}
	if (move) {
	
		if (this->ObjectOBJ->getPosition().x <= position1.x)
		{
			move = false;
		}
		else {
			XMFLOAT3 firstVector = MULT(current, secondriktningsVector);
			XMFLOAT3 Vector = MULT(firstVector, dt/5);
			ObjectOBJ->getRigidbody()->setLinearVelocity(btVector3(Vector.x*50, Vector.y*50, 0));
		}
	}

}
Objects::Objects()
{
	this->id = 1;
}
Objects::Objects(std::string filepath, btVector3 position,int id,int friction, btVector3 size, OBJECTSTATE state, OBJECTYPE type) :state(state), type(type)
{
	this->id = id;
	this->ObjectOBJ = new GameObject(System::shaderManager->getForwardShader());

	System::theModelLoader->loadGO(this->ObjectOBJ, filepath.c_str());
	System::handler->addObject(this->ObjectOBJ);
	this->ObjectOBJ->setPosition(btVector3(position.getX(), position.getY()+0.57f, position.getZ()));
	
	this->position1 = XMFLOAT3(position.getX(), position.getY(), position.getZ());
	this->position2 = XMFLOAT3(position1.x + 5, position1.y+5, position1.z);

	firstriktningsVector = normalize(VECTORSUBTRACTION(position2,position1));
	secondriktningsVector = normalize(VECTORSUBTRACTION(position1,position2));
	
	if(size.getX() != 2.04f)
		this->ObjectOBJ->setScale(size);
	this->move = true;
	//AABB aabb = ObjectOBJ->getCollisionBox();
	//btVector3 sizeOfOBject = btVector3(aabb.width, aabb.height, 1);
	btVector3 PositionOfOBject = ObjectOBJ->positionOffset;

	AABB aabb = ObjectOBJ->getCollisionBox();
	btVector3(aabb.width, aabb.height, aabb.width);
	if (state == DYNAMIC) {
		this->ObjectOBJ->getRigidbody() = System::getphysices()->addBox(btVector3(position),
			btVector3(ObjectOBJ->getCollisionBox().width * 2, ObjectOBJ->getCollisionBox().height * 2, ObjectOBJ->getCollisionBox().depth * 2), 10000000.0f, this);
		this->ObjectOBJ->getRigidbody()->setGravity(btVector3(0, 0, 0));
	}
	else if (state == STATIC) {
		this->ObjectOBJ->getRigidbody() = System::getphysices()->addBox(btVector3(position),
			btVector3(ObjectOBJ->getCollisionBox().width * 2, ObjectOBJ->getCollisionBox().height * 2, ObjectOBJ->getCollisionBox().depth * 2), 10000000.0f, this);
		this->ObjectOBJ->getRigidbody()->setGravity(btVector3(0, 0, 0));
	}
	else {
		this->ObjectOBJ->getRigidbody() = System::getphysices()->addBox(btVector3(position),
			btVector3(ObjectOBJ->getCollisionBox().width * 2, ObjectOBJ->getCollisionBox().height * 2, ObjectOBJ->getCollisionBox().depth * 2), 0.0f, this);
	}

	if (state == STATIC) {
		ObjectOBJ->getRigidbody()->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	}
	else {
		ObjectOBJ->getRigidbody()->setCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	}


	this->ObjectOBJ->getRigidbody()->setActivationState(DISABLE_DEACTIVATION);
	this->ObjectOBJ->getRigidbody()->setFriction(friction);
	this->ObjectOBJ->getRigidbody()->setRestitution(0);
	this->ObjectOBJ->getRigidbody()->setAngularFactor(btVector3(0, 0, 0));
}

Objects::~Objects()
{
}

void Objects::update(float dt)
{
	
	this->ObjectOBJ->setPosition(this->ObjectOBJ->GetPosition().x, this->ObjectOBJ->GetPosition().y + 0.57f,
		this->ObjectOBJ->GetPosition().z);


	btRigidBody* rgb = this->ObjectOBJ->getRigidbody();
	switch (type)
	{
		case PLATFORM:
			if (state == STATIC){

			}
			else if (state == DYNAMIC){

			}
			break;
		case STONE:
			if (state == STATIC){
				
			}
			else if (state == DYNAMIC){
				this->SimplePlatformMovement(dt);
			}
			break;
		case TREE:
			if (state == STATIC){

			}
			else if (state == DYNAMIC){
				
			}
			break;
		default:
			break;
	}

}

void Objects::setMovement(bool move)
{
	this->move = move;
}

int Objects::getId()
{
	return this->id;
}

XMFLOAT3 Objects::getMovingSpeed()
{
	return XMFLOAT3(this->ObjectOBJ->getRigidbody()->getLinearVelocity().getX(), this->ObjectOBJ->getRigidbody()->getLinearVelocity().getY(), this->ObjectOBJ->getRigidbody()->getLinearVelocity().getZ());
}
