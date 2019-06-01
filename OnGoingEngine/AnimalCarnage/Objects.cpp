#include "Objects.h"
#include "System.h"
#include <string>
#include <random>
#include <iostream>
void Objects::SimplePlatformMovement(float dt)
{
	if (state != BACKGROUND) {
		XMFLOAT3 current = ObjectOBJ->getPosition();

		if (move == false) {

			if (this->ObjectOBJ->getPosition().x > position2.x) {
				move = true;

			}
			else {
				XMFLOAT3 firstVector = MULT(current, firstriktningsVector);
				XMFLOAT3 Vector = MULT(firstVector, dt / 5);
				ObjectOBJ->getRigidbody()->setLinearVelocity(btVector3(Vector.x * 50, Vector.y * 0, 0));
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
				XMFLOAT3 Vector = MULT(firstVector, dt / 5);
				ObjectOBJ->getRigidbody()->setLinearVelocity(btVector3(Vector.x * 50, Vector.y * 50, 0));
				/*if (type == PLATFORM && state == DYNAMIC) {
					ObjectOBJPlatform->getRigidbody()->setLinearVelocity(btVector3(Vector.x * 50, Vector.y * 50, 0));
				}*/
			}

		}


	}
}
Objects::Objects()
{
	this->id = 1;
	this->rotationSpeed = 0;
}
Objects::Objects(std::string filepath, btVector3 position,int id,int friction, btVector3 size, btVector3 rotation, OBJECTSTATE state, OBJECTYPE type,int mipLevels, float x, float y, bool changeOpacity,bool activeDraw) :state(state), type(type)
{
	this->friction = friction;
	this->direction = 0;
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
	this->ObjectOBJ->setRotationRollPitchYaw(DirectX::XMConvertToRadians(rotation.getX()), DirectX::XMConvertToRadians(rotation.getY()), DirectX::XMConvertToRadians(rotation.getZ()));
	this->ObjectOBJ->setRepeat(x, y);//repeated texture
	this->position1 = XMFLOAT3(position.getX(), position.getY(), position.getZ());
	this->position2 = XMFLOAT3(position1.x + 5, position1.y +0.0005, position1.z);

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
			btVector3(ObjectOBJ->getCollisionBox().width * 2, ObjectOBJ->getCollisionBox().height * 2, ObjectOBJ->getCollisionBox().depth * 2), 100.0f, this);
	}
	else if (state == BACKGROUND) {

	}
	else {
		this->ObjectOBJ->getRigidbody() = System::getphysices()->addBox(btVector3(position),
			btVector3(ObjectOBJ->getCollisionBox().width * 2, ObjectOBJ->getCollisionBox().height * 2, ObjectOBJ->getCollisionBox().depth * 2), 0.0f, this);
	}

	if (state == STATIC) {
		ObjectOBJ->getRigidbody()->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	}
	else if (state == BACKGROUND) {

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
	if (state == BACKGROUND) {

	}
	else if (state != TRUE_DYNAMIC) {
		this->ObjectOBJ->getRigidbody()->setActivationState(DISABLE_DEACTIVATION);
		this->ObjectOBJ->getRigidbody()->setFriction(friction);
		this->ObjectOBJ->getRigidbody()->setAngularFactor(btVector3(0, 0, 0));
		this->ObjectOBJ->getRigidbody()->setRestitution(0);
	}
	else if (state == TRUE_DYNAMIC) {
		//this->ObjectOBJ->getRigidbody()->setActivationState(DISABLE_DEACTIVATION);
		this->ObjectOBJ->getRigidbody()->setFriction(friction);
		this->ObjectOBJ->getRigidbody()->setAngularFactor(btVector3(0, 0, 1));
		this->ObjectOBJ->getRigidbody()->setLinearFactor(btVector3(1, 1, 0));
		this->ObjectOBJ->getRigidbody()->setRestitution(0.5);
		this->ObjectOBJ->getRigidbody()->setGravity(btVector3(0, -30, 0));
	}
	this->maxHealth = 100;
	this->health = 100;
	this->respawnTimer = 0;
	this->hitTimer = 0;
	this->cornerTimer = 0;
	this->impactTimer = 0;
	this->canBeImpacted = true;
	//this->ObjectOBJ->getRigidbody()->setAngularFactor(btVector3(0, 0, 0));
	this->lastPlayerHit = -1;
	this->playerKilled = -1;
}

Objects::~Objects()
{
}

void Objects::update(float dt)
{

	if (type == GRASS) {
		ObjectOBJ->setRotationRollPitchYaw(0.5, 0, ObjectOBJ->getRotation().z + (rotationSpeed * dt * 60));
		if (ObjectOBJ->getRotation().z > (3.14 / 2) - 0.1) {
			ObjectOBJ->setRotationRollPitchYaw(0.5, 0, (3.14 / 2) - 0.1);
		}
		if (ObjectOBJ->getRotation().z < -1.57079633 + 0.1) {
			ObjectOBJ->setRotationRollPitchYaw(0.5, 0, -1.57079633 + 0.1);
		}
		if (ObjectOBJ->getRotation().z < 0) {
			rotationSpeed += 0.005 /*+ (abs(rotationSpeed)/25)*/;
		}
		else if (ObjectOBJ->getRotation().z >= 0) {
			rotationSpeed -= 0.005 /*+ (abs(rotationSpeed)/25)*/;
		}
		rotationSpeed /= 1.05;
		if (rotationSpeed > 0.1) {
			rotationSpeed = 0.1;
		}
		if (rotationSpeed < -0.1) {
			rotationSpeed = -0.1;
		}

		ObjectOBJ->setPosition(position1.x, position1.y - (abs(ObjectOBJ->getRotation().z))/3 +0.57, position1.z);
	}

	//Respawn when Stone is dead

	if (state == TRUE_DYNAMIC) 
	{
		if (abs(getMovingSpeed().x) > 10 || abs(getMovingSpeed().z) > 10)
		{
			System::getParticleManager()->addSimpleEffect(DirectX::SimpleMath::Vector3(ObjectOBJ->GetPosition().x, ObjectOBJ->GetPosition().y+1.5, ObjectOBJ->GetPosition().z), "hit_effect", 2.2f, 0.8f, false, 1, 0.1, DirectX::SimpleMath::Vector3(0,0,0),-0.03);
		}


		this->impactTimer += 1000 * dt;
		this->activeTimer += 700 * dt;
		if (activeTimer >= 100) {
			canGiveDmg = true;
			this->ObjectOBJ->getRigidbody()->setFriction(friction);
		}
		else {
			canGiveDmg = false;
			this->ObjectOBJ->getRigidbody()->setFriction(0);
		}


		if (this->health <= 0) {
			if (respawnTimer <= 0) {
				//System::getParticleManager()->addSimpleEffect(DirectX::SimpleMath::Vector3(ObjectOBJ->getPosition()), "rumble",1 , 2.0f,true,30);
				System::getParticleManager()->addSimpleEffect(DirectX::SimpleMath::Vector3(ObjectOBJ->getPosition()), "rumble", 1, 2.0f, true, 25, 5,
					DirectX::SimpleMath::Vector3(getMovingSpeed().x / 2, getMovingSpeed().y / 2, getMovingSpeed().z / 2), -0.001);
			}
			respawnTimer += 40 * dt;
			this->ObjectOBJ->getRigidbody()->getWorldTransform().setOrigin(btVector3(this->position1.x, this->position1.y - 100, this->position1.z));

				if (respawnTimer >= 100) {
					respawnTimer = 0;
					respawn(50);
				}
			}
		if (this->health >= 100) {
			this->health = 100;
		}
		if (this->getMovingSpeed().x > 1.5) {
			direction = 1;
		}else if(this->getMovingSpeed().x < -1.5) {
			direction = -1;
		}
		
		hitTimer += 700 * dt;
		if (hitTimer >= 100) {
			canBeHit = true;
		}
		else {
			canBeHit = false;
		}

		if (getRigidBodyPosition().getX() > 28 || getRigidBodyPosition().getX() < -28) {
			cornerTimer += 50 * dt;
			if (cornerTimer >= 100) {
				respawn(50);
			}
		}
		else {
			cornerTimer = 0;
		}
	}

	if (state != BACKGROUND) {
		if (this->ObjectOBJ != nullptr)
			this->ObjectOBJ->setPosition(this->ObjectOBJ->GetPosition().x, this->ObjectOBJ->GetPosition().y + 0.57f,
				this->ObjectOBJ->GetPosition().z);
	}
	if (type == GROUND) {
		this->ObjectOBJ->setPosition(this->ObjectOBJ->GetPosition().x, this->ObjectOBJ->GetPosition().y + 0.9f,
			this->ObjectOBJ->GetPosition().z);
	}
	if (state == TRUE_DYNAMIC) {
		XMVECTOR temp;
		temp.m128_f32[0] = this->ObjectOBJ->getRigidbody()->getWorldTransform().getRotation().getX();
		temp.m128_f32[1] = this->ObjectOBJ->getRigidbody()->getWorldTransform().getRotation().getY();
		temp.m128_f32[2] = this->ObjectOBJ->getRigidbody()->getWorldTransform().getRotation().getZ();
		temp.m128_f32[3] = this->ObjectOBJ->getRigidbody()->getWorldTransform().getRotation().getW();

		ObjectOBJ->setRotationRollPitchYaw(ObjectOBJ->getRotation().x, ObjectOBJ->getRotation().y, getRoll(temp));
		//getRoll(temp) * -1
	}


	
	if (state != BACKGROUND) {
		btRigidBody* rgb = this->ObjectOBJ->getRigidbody();
	}
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

void Objects::respawn(float offset)
{
	if (state != BACKGROUND) {
		this->ObjectOBJ->getRigidbody()->setLinearVelocity(btVector3(0, 0, 0));
		this->ObjectOBJ->getRigidbody()->getWorldTransform().setOrigin(btVector3(this->position1.x, this->position1.y + offset, this->position1.z));
		//this->health=maxHealth;
			//this->respawnPoints[i].respawnObject->ObjectOBJ->getRigidbody()->getWorldTransform().setOrigin(this->respawnPoints[i].firstSpawn);
		this->health = 100;
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

int Objects::getMovingDirection()
{
	return this->direction;
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

void Objects::addImpulse(float impulse, int playerId)
{
	if (state != BACKGROUND && canBeHit == true) {
		this->lastPlayerHit = playerId;
		canGiveDmg = false;
		activeTimer = 0;
		ObjectOBJ->getRigidbody()->activate();
		ObjectOBJ->getRigidbody()->applyImpulse(btVector3(impulse*50, 0, 0), btVector3(0, 0, 0));
		System::getSoundManager()->playEffect("Stone_Getting_Hit");
		//System::getParticleManager()->addSimpleEffect(DirectX::SimpleMath::Vector3(ObjectOBJ->getPosition()), "rumble", 1, 1.0f, true, 30,10);
		this->canBeHit = false;
		this->hitTimer = 0;
	}
}

bool Objects::getCanGiveDmg() const
{
	return this->canGiveDmg;
}

void Objects::takeDmg(int damage)
{
	this->health -= damage;
}

void Objects::addGrassRotation(float addRotation, int dir)
{
	if (ObjectOBJ->getRotation().z > (3.14 / 2) - 0.2 || ObjectOBJ->getRotation().z < -1.57079633 + 0.2) {
		rotationSpeed = rotationSpeed * 2;
	}
	else {
		if (dir == 1) {
			rotationSpeed -= addRotation;
		}
		if (dir == -1) {
			rotationSpeed += addRotation;
		}
	}
}

XMFLOAT3 Objects::getPosition()
{
	return this->position1;
}

btVector3 Objects::getRigidBodyPosition()
{
	return this->ObjectOBJ->getRigidbody()->getWorldTransform().getOrigin();
}

void Objects::setLastPlayerHit(int lastPlayerHit)
{
	this->lastPlayerHit = lastPlayerHit;
}

int Objects::getLastPlayerHit()
{
	return lastPlayerHit;
}

void Objects::setPlayerKilled(bool playerKilled)
{
	this->playerKilled = playerKilled;
}

bool Objects::getPlayerKilled()
{
	return this->playerKilled;
}

void Objects::impactSoundEffect()
{
	if (impactTimer >= 100) {
		if ((abs(getMovingSpeed().x) > 10 || abs(getMovingSpeed().y) > 10)) {
			System::getSoundManager()->playEffect("Stone_Hard_Impact");
			System::getParticleManager()->addSimpleEffect(DirectX::SimpleMath::Vector3(ObjectOBJ->getPosition()), "dirt", 0.2f, 0.5f, true, 15, 5,
				DirectX::SimpleMath::Vector3(getMovingSpeed().x / 5, getMovingSpeed().y / 5, getMovingSpeed().z / 5), -0.005);
		}
		else if ((abs(getMovingSpeed().x) > 5 || abs(getMovingSpeed().y) > 5)) {
			System::getSoundManager()->playEffect("Stone_Soft_Impact");
		}
		impactTimer = 0;
	}
}

int Objects::getHealth()
{
	return this->health;
}
