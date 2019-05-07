#include "Objects.h"
#include "System.h"

Objects::Objects(std::string filepath, std::string texture, btVector3 position,btVector3 size,OBJECTSTATE state, OBJECTYPE type) :state(state), type(type)
{
	this->ObjectOBJ = new GameObject(System::shaderManager->getForwardShader());
	System::theModelLoader->loadGO(this->ObjectOBJ, filepath.c_str(), texture);
	System::handler->addObject(this->ObjectOBJ);

	this->ObjectOBJ->getRigidbody() = System::getphysices()->addBox(position, size, 0.0f);
	//this->playerObj->getRigidbody()->getWorldTransform().setRotation(btQuaternion(3.14 / 2, 0, 0));
	this->ObjectOBJ->getRigidbody()->setWorldTransform(XMMATRIX_to_btTransform(this->ObjectOBJ->getWorld()));
	this->ObjectOBJ->setRotationRollPitchYaw(0.f, 3.14f / 2.f, 0.f);

	float halfsize[3] = { size.getX()/2,size.getY()/2,size.getZ()/2 };
	float pos[3] = { position.getX(),position.getY(),position.getZ() };
	this->ObjectOBJ->setHalfSize(halfsize, pos);
	if(state ==STATIC)
		ObjectOBJ->getRigidbody()->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);

	this->ObjectOBJ->getRigidbody()->setActivationState(DISABLE_DEACTIVATION);
	this->ObjectOBJ->getRigidbody()->setFriction(0.5);
	this->ObjectOBJ->getRigidbody()->setAngularFactor(btVector3(0, 0, 0));
}

Objects::~Objects()
{
}

void Objects::update()
{
	this->ObjectOBJ->setPosition(this->ObjectOBJ->getRigidbody()->getWorldTransform().getOrigin().getX()
		, this->ObjectOBJ->getRigidbody()->getWorldTransform().getOrigin().getY(), this->ObjectOBJ->getRigidbody()->getWorldTransform().getOrigin().getZ());
	if (state == STATIC) 
	{
		if (type == STONE)
		{

		}

	}
}
