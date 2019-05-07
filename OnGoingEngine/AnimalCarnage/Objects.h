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
public:
	Objects(std::string filepath, std::string texture  ,btVector3 position, btVector3 size = btVector3(1,1,1),OBJECTSTATE state = STATIC, OBJECTYPE type = STONE);
	~Objects();
	void update();
	GameObject* ObjectOBJ;
	btRigidBody * GetRigidBody() { return this->ObjectOBJ->getRigidbody(); };
};

