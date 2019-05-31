#pragma once
#include "GameObject.h"
enum OBJECTYPE {
	PLATFORM,STONE,TREE,GRASS, GROUND
};
enum OBJECTSTATE {
STATIC,DYNAMIC,TRUE_DYNAMIC,BACKGROUND
};
class Objects
{
private:
	OBJECTSTATE state;//static /dynamic
	OBJECTYPE type;
	void SimplePlatformMovement(float dt);
	XMFLOAT3 position1;
	XMFLOAT3 position2;
	bool move;
	int id;

	XMFLOAT3 firstriktningsVector;
	XMFLOAT3 secondriktningsVector;
	float activeTimer;
	bool canGiveDmg;
	int health;
	int maxHealth;
	float respawnTimer;
	float hitTimer;
	bool canBeHit;
	float cornerTimer;
	float impactTimer;
	bool canBeImpacted;
	int lastPlayerHit;
	bool playerKilled;

	float rotationSpeed;
	int direction;
	float friction;
	

public:
	Objects();
	Objects(std::string filepath ,btVector3 position, int id, int friction, btVector3 size = btVector3(1,1,1), btVector3 rotation = btVector3(0, 0, 0), OBJECTSTATE state = STATIC, OBJECTYPE type = STONE,int mipLevels=-1, float x=1, float y=1,bool changeOpacity=false,bool activeDraw=true);
	~Objects();
	void update(float dt);
	void respawn(float offset);
	GameObject* ObjectOBJ;
	/*GameObject* ObjectOBJPlatform;*/
	btRigidBody * GetRigidBody() { return this->ObjectOBJ->getRigidbody(); };
	void setMovement(bool move =true);
	int getId();
	XMFLOAT3 getMovingSpeed();
	int getMovingDirection();
	OBJECTYPE GetType() { return this->type; }
	OBJECTSTATE GetState() { return this->state; }
	
	float getPitch(DirectX::XMVECTOR Quaternion);
	float getYaw(DirectX::XMVECTOR Quaternion);
	float getRoll(DirectX::XMVECTOR Quaternion);

	void addImpulse(float impulse, int playerId);
	bool getCanGiveDmg() const;
	void takeDmg(int damage);

	void addGrassRotation(float addRotation, int dir);
	XMFLOAT3 getPosition();
	btVector3 getRigidBodyPosition();
	void setLastPlayerHit(int lastPlayerHit);
	int getLastPlayerHit();
	void setPlayerKilled(bool playerKilled);
	bool getPlayerKilled();
	void impactSoundEffect();

	int getHealth();
};

