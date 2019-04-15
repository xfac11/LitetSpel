#ifndef GAMEOBJECTHANDLER_H
#define GAMEOBJECTHANDLER_H
#include"GameObject.h"
class GameObjectHandler
{
public:
	GameObjectHandler();
	~GameObjectHandler();
	void addObject(ShaderType type, GameObject*& gameObject);
	void addObject();
private:
	GameObject* *gameObjects;
	int cap;
	int nrOfObjects;
	struct ModWorld
	{
		DirectX::XMMATRIX* worldPtr;
		Model* modelPtr;
	};
	ModWorld* transModels;
	ModWorld* opaqueModels;

	void expand();
};
#endif // !GAMEOBJECTHANDLER_H
