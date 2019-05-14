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

			ObjectOBJ->Move(Vector);
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
			ObjectOBJ->Move(Vector);
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
	this->ObjectOBJ->setPosition(btVector3(position.getX(), position.getY(), position.getZ()));
	this->ObjectOBJ->setScale(size);

	this->position1 = XMFLOAT3(position.getX(), position.getY(), position.getZ());
	this->position2 = XMFLOAT3(position1.x + 5, position1.y+5, position1.z);

	firstriktningsVector = normalize(VECTORSUBTRACTION(position2,position1));
	secondriktningsVector = normalize(VECTORSUBTRACTION(position1,position2));

	this->move = true;
	//AABB aabb = ObjectOBJ->getCollisionBox();
	//btVector3 sizeOfOBject = btVector3(aabb.width, aabb.height, 1);
	btVector3 PositionOfOBject = ObjectOBJ->positionOffset;

	this->ObjectOBJ->getRigidbody() = System::getphysices()->addBox(btVector3(position.getX(), position.getY(), position.getZ()),btVector3(size.getX()/2.0f,size.getY()/2.0f,size.getZ()/2.0f), 0.0f,this);
	//this->ObjectOBJ->setScale(size);
	//this->playerObj->getRigidbody()->getWorldTransform().setRotation(btQuaternion(3.14 / 2, 0, 0));
	//this->ObjectOBJ->getRigidbody()->setWorldTransform(XMMATRIX_to_btTransform(this->ObjectOBJ->getWorld()));
	//this->ObjectOBJ->setRotationRollPitchYaw(0.f, 3.14f / 2.f, 0.f);

	//float halfsize[3] = { size.getX()/2,size.getY()/2,size.getZ()/2 };
	//float pos[3] = { position.getX(),position.getY(),position.getZ()};
	//this->ObjectOBJ->setHalfSize(halfsize, pos);
	if(state ==STATIC)
		ObjectOBJ->getRigidbody()->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

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
	this->ObjectOBJ->setPosition(this->ObjectOBJ->getRigidbody()->getWorldTransform().getOrigin().getX()
		, this->ObjectOBJ->getRigidbody()->getWorldTransform().getOrigin().getY(), this->ObjectOBJ->getRigidbody()->getWorldTransform().getOrigin().getZ());

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
