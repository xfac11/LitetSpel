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
	void initialize();
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
	struct AnyLight
	{
		DirectX::XMMATRIX worldLight;
		float position[4]; // w/a = Radius/Range
		float color[4]; // w/a = intensity
		float cameraPos[4];// for specular calc w/a = numberoflights
		float direction[4];// directional light
	};
	struct TheLights
	{
		int nrOfLights;
		AnyLight lights[16];
	};
	ModWorld* transModels;
	ModWorld* opaqueModels;
	ConstantBuffer<TheLights> lightsCB;
	void expandGameobjects();
	void expandTransModels();
	void expandOpaqueModels();
};

#endif // !GAMEOBJECTHANDLER_H
