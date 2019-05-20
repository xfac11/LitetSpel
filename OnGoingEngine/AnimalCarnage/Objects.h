#pragma once
#include "GameObject.h"
enum OBJECTYPE {
	PLATFORM,STONE,TREE
};
enum OBJECTSTATE {
STATIC,DYNAMIC
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
public:
	Objects();
	Objects(std::string filepath ,btVector3 position, int id, int friction, btVector3 size = btVector3(1,1,1), OBJECTSTATE state = STATIC, OBJECTYPE type = STONE,int mipLevels=-1, float x=1, float y=1);
	~Objects();
	void update(float dt);
	GameObject* ObjectOBJ;
	/*GameObject* ObjectOBJPlatform;*/
	btRigidBody * GetRigidBody() { return this->ObjectOBJ->getRigidbody(); };
	void setMovement(bool move =true);
	int getId();
	XMFLOAT3 getMovingSpeed();
	OBJECTYPE GetType() { return this->type; }
	OBJECTSTATE GetState() { return this->state; }

};

