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

			ObjectOBJ->getRigidbody()->setLinearVelocity(btVector3(Vector.x * 50, Vector.y *0,0));
			/*if (type == PLATFORM && state == DYNAMIC) {
				ObjectOBJPlatform->getRigidbody()->setLinearVelocity(btVector3(Vector.x * 50, Vector.y * 50, 0));
			}*/

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
			ObjectOBJ->getRigidbody()->setLinearVelocity(btVector3(Vector.x*50, Vector.y*0, 0));
			/*if (type == PLATFORM && state == DYNAMIC) {
				ObjectOBJPlatform->getRigidbody()->setLinearVelocity(btVector3(Vector.x * 50, Vector.y * 50, 0));
			}*/
		}
	}

}
Objects::Objects()
{
	this->id = 1;
}
Objects::Objects(std::string filepath, btVector3 position,int id,int friction, btVector3 size, OBJECTSTATE state, OBJECTYPE type,int mipLevels, float x, float y, bool changeOpacity,bool activeDraw) :state(state), type(type)
{
	this->activeTimer = 0;
	this->canGiveDmg = true;
	this->id = id;
	this->ObjectOBJ = new GameObject(System::shaderManager->getForwardShader());
	
	System::theModelLoader->loadGO(this->ObjectOBJ, filepath.c_str(),mipLevels);
	if (changeOpacity)
		this->ObjectOBJ->getModel()->setOpacity(Opaque);
	this->ObjectOBJ->setActiveDraw(activeDraw);
	System::handler->addObject(this->ObjectOBJ);
	this->ObjectOBJ->setPosition(btVector3(position.getX(), position.getY()+0.57f, position.getZ()));
	this->ObjectOBJ->setRepeat(x, y);//repeated texture
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
	else if (state == TRUE_DYNAMIC) {
		this->ObjectOBJ->getRigidbody() = System::getphysices()->addBox(btVector3(position),
			btVector3(ObjectOBJ->getCollisionBox().width * 2, ObjectOBJ->getCollisionBox().height * 2, ObjectOBJ->getCollisionBox().depth * 2), 20.0f, this);
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

	/*if (state == TRUE_DYNAMIC) {
		ObjectOBJ->getRigidbody()->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	}
*/
	/*if (type == PLATFORM) {
		this->ObjectOBJPlatform = new GameObject(System::shaderManager->getForwardShader());

		System::theModelLoader->loadGO(this->ObjectOBJPlatform, filepath.c_str());
		System::handler->addObject(this->ObjectOBJPlatform);
		this->ObjectOBJPlatform->setPosition(btVector3(position.getX(), position.getY() + 0.57f, position.getZ()));

		if (size.getX() != 2.04f)
			this->ObjectOBJPlatform->setScale(size);

			if (state == DYNAMIC) {
				this->ObjectOBJPlatform->getRigidbody() = System::getphysices()->addBox(btVector3(position),
					btVector3(ObjectOBJPlatform->getCollisionBox().width * 2 + 0.1, ObjectOBJPlatform->getCollisionBox().height * 2, ObjectOBJPlatform->getCollisionBox().depth * 2), 10000000.0f, this);
				this->ObjectOBJPlatform->getRigidbody()->setGravity(btVector3(0, 0, 0));
			}
			else if (state == STATIC) {
				this->ObjectOBJPlatform->getRigidbody() = System::getphysices()->addBox(btVector3(position),
					btVector3(ObjectOBJPlatform->getCollisionBox().width * 2 + 0.1, ObjectOBJPlatform->getCollisionBox().height * 2, ObjectOBJPlatform->getCollisionBox().depth * 2), 10000000.0f, this);
				this->ObjectOBJPlatform->getRigidbody()->setGravity(btVector3(0, 0, 0));
				ObjectOBJPlatform->getRigidbody()->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
			}
			else {
				this->ObjectOBJPlatform->getRigidbody() = System::getphysices()->addBox(btVector3(position),
					btVector3(ObjectOBJPlatform->getCollisionBox().width * 2 + 0.1, ObjectOBJPlatform->getCollisionBox().height * 2, ObjectOBJPlatform->getCollisionBox().depth * 2), 0.0f, this);
			}


		this->ObjectOBJPlatform->getRigidbody()->setActivationState(DISABLE_DEACTIVATION);
		this->ObjectOBJPlatform->getRigidbody()->setFriction(0);
		this->ObjectOBJPlatform->getRigidbody()->setRestitution(0);
		this->ObjectOBJPlatform->getRigidbody()->setAngularFactor(btVector3(0, 0, 0));
	}*/

	this->ObjectOBJ->getRigidbody()->setActivationState(DISABLE_DEACTIVATION);
	this->ObjectOBJ->getRigidbody()->setFriction(friction);
	if (state != TRUE_DYNAMIC) {
		this->ObjectOBJ->getRigidbody()->setAngularFactor(btVector3(0, 0, 0));
		this->ObjectOBJ->getRigidbody()->setRestitution(0);
	}
	else if (state == TRUE_DYNAMIC) {
		this->ObjectOBJ->getRigidbody()->setAngularFactor(btVector3(0, 0, 1));
		this->ObjectOBJ->getRigidbody()->setLinearFactor(btVector3(1, 1, 0));
		this->ObjectOBJ->getRigidbody()->setRestitution(0.5);
		this->ObjectOBJ->getRigidbody()->setGravity(btVector3(0, -20, 0));
	}
	this->maxHealth = 100;
	this->health = 100;
	this->respawnTimer = 0;
	//this->ObjectOBJ->getRigidbody()->setAngularFactor(btVector3(0, 0, 0));
}

Objects::~Objects()
{
}

void Objects::update(float dt)
{
	this->activeTimer += 7500 * dt;
	if (activeTimer >= 100) {
		canGiveDmg = true;
	}
	else {
		canGiveDmg = false;
	}

	//Respawn when Stone is dead
	if (this->health <= 0) {
		if (respawnTimer <= 0) {
			System::getParticleManager()->addSimpleEffect(DirectX::SimpleMath::Vector3(ObjectOBJ->getRigidbody()->getWorldTransform().getOrigin().getX(), ObjectOBJ->getRigidbody()->getWorldTransform().getOrigin().getY(), ObjectOBJ->getRigidbody()->getWorldTransform().getOrigin().getZ()), "rumble",3.0f,1.0f,10);
		}
		respawnTimer += 40 * dt;
		this->ObjectOBJ->getRigidbody()->getWorldTransform().setOrigin(btVector3(this->position1.x, this->position1.y - 100, this->position1.z));

		if (respawnTimer >= 100) {
			respawnTimer = 0;
			respawn();
		}
	}
	if (this->health >= 100) {
		this->health = 100;
	}


	if(this->ObjectOBJ != nullptr)
		this->ObjectOBJ->setPosition(this->ObjectOBJ->GetPosition().x, this->ObjectOBJ->GetPosition().y + 0.57f,
		this->ObjectOBJ->GetPosition().z);
	if (state == TRUE_DYNAMIC) {
		XMVECTOR temp;
		temp.m128_f32[0] = this->ObjectOBJ->getRigidbody()->getWorldTransform().getRotation().getX();
		temp.m128_f32[1] = this->ObjectOBJ->getRigidbody()->getWorldTransform().getRotation().getY();
		temp.m128_f32[2] = this->ObjectOBJ->getRigidbody()->getWorldTransform().getRotation().getZ();
		temp.m128_f32[3] = this->ObjectOBJ->getRigidbody()->getWorldTransform().getRotation().getW();

		ObjectOBJ->setRotationRollPitchYaw(ObjectOBJ->getRotation().x, ObjectOBJ->getRotation().y, getRoll(temp));
		//getRoll(temp) * -1
	}


	
	
	btRigidBody* rgb = this->ObjectOBJ->getRigidbody();
	switch (type)
	{
		case PLATFORM:
			/*this->ObjectOBJPlatform->setPosition(this->ObjectOBJPlatform->GetPosition().x, this->ObjectOBJPlatform->GetPosition().y + 0.57,
				this->ObjectOBJPlatform->GetPosition().z);*/
			if (state == STATIC){

			}
			else if (state == DYNAMIC){
				this->SimplePlatformMovement(dt);
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

void Objects::respawn()
{
	this->ObjectOBJ->getRigidbody()->setLinearVelocity(btVector3(0, 0, 0));
	this->ObjectOBJ->getRigidbody()->getWorldTransform().setOrigin(btVector3(this->position1.x, this->position1.y, this->position1.z));
	//this->health=maxHealth;
		//this->respawnPoints[i].respawnObject->ObjectOBJ->getRigidbody()->getWorldTransform().setOrigin(this->respawnPoints[i].firstSpawn);
	this->health = 100;
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

float Objects::getPitch(DirectX::XMVECTOR Quaternion)
{
	return atan2(2 * (Quaternion.m128_f32[1] * Quaternion.m128_f32[2] + Quaternion.m128_f32[3] * Quaternion.m128_f32[0]), Quaternion.m128_f32[3] * Quaternion.m128_f32[3] - Quaternion.m128_f32[0] * Quaternion.m128_f32[0] - Quaternion.m128_f32[1] * Quaternion.m128_f32[1] + Quaternion.m128_f32[2] * Quaternion.m128_f32[2]);
}

float Objects::getYaw(DirectX::XMVECTOR Quaternion)
{
	return asin(-2 * (Quaternion.m128_f32[0] * Quaternion.m128_f32[2] - Quaternion.m128_f32[3] * Quaternion.m128_f32[1]));
}

float Objects::getRoll(DirectX::XMVECTOR Quaternion)
{
	return atan2(2 * (Quaternion.m128_f32[0] * Quaternion.m128_f32[1] + Quaternion.m128_f32[3] * Quaternion.m128_f32[2]), Quaternion.m128_f32[3] * Quaternion.m128_f32[3] + Quaternion.m128_f32[0] * Quaternion.m128_f32[0] - Quaternion.m128_f32[1] * Quaternion.m128_f32[1] - Quaternion.m128_f32[2] * Quaternion.m128_f32[2]);
}

void Objects::addImpulse(float impulse)
{
	canGiveDmg = false;
	activeTimer = 0;
	ObjectOBJ->getRigidbody()->applyImpulse(btVector3(impulse,10,0),btVector3(1,0,0));
}

bool Objects::getCanGiveDmg() const
{
	return this->canGiveDmg;
}

void Objects::takeDmg(int damage)
{
	this->health -= damage;
}
