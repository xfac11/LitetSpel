#ifndef GAMEOBJECTHANDLER_H
#define GAMEOBJECTHANDLER_H
#include"GameObject.h"
class GameObjectHandler
{
public:
	GameObjectHandler();
	~GameObjectHandler();
	void addObject(GameObject*& gameObject);
	void addObject(char *file);
	GameObject& getObject(int id);
	void draw();
private:
	GameObject* *gameObjects;
	int cap;
	int nrOfObjects;
	int capOpaque;
	int nrOfOpaque;
	int capTrans;
	int nrOfTrans;
	struct ModWorld
	{
		DirectX::XMMATRIX* worldPtr;
		Model* modelPtr;
	};
	ModWorld* transModels;
	ModWorld* opaqueModels;

	void expandGameobjects();
	void expandTransModels();
	void expandOpaqueModels();
};

#endif // !GAMEOBJECTHANDLER_H
