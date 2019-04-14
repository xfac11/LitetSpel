#ifndef GAMEOBJECTHANDLER_H
#define GAMEOBJECTHANDLER_H
#include"GameObject.h"
class GameObjectHandler
{
public:
	GameObjectHandler();
	~GameObjectHandler();
	void addObject();

private:
	GameObject* *gameObjects;
	int nrOfObjects;
	struct ModWorld
	{
		DirectX::XMMATRIX* worldPtr;
		Model* modelPtr;
	};
	ModWorld* modelsSorted;
};
#endif // !GAMEOBJECTHANDLER_H
