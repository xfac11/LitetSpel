#ifndef GAMEOBJECTHANDLER_H
#define GAMEOBJECTHANDLER_H
#include"GameObject.h"
#include"Skybox.h"
class GameObjectHandler
{
public:
	GameObjectHandler();
	~GameObjectHandler();
	void addObject(GameObject*& gameObject);
	void addObject(char *file);
	GameObject& getObject(int id);
	void draw(float deltaTime);
	void initialize();
	void addLight(float pos[4], float dir[4], float color[4]);
	void setSkyboxTexture(std::string file);
	void setSunDir(DirectX::XMFLOAT3 dir);
private:
	GameObject* *gameObjects;
	int cap;
	int nrOfObjects;
	int capOpaque;
	int nrOfOpaque;
	int capTrans;
	int nrOfTrans;
	int nrOfLights;

	int animTimer;
	struct ModWorld
	{
		DirectX::XMMATRIX* worldPtr;
		Model* modelPtr;
	};

	
	struct LightVertex
	{
		float position[3];
		float uv[2];
	};
	std::vector<LightVertex> quad;
	VertexBuffer<LightVertex> vertexBufferQuad;


	std::vector<LightVertex> sphereVector;
	std::vector<DWORD> sphereIndices;

	IndexBuffer indexBufferSphere;
	VertexBuffer<LightVertex> vertexBufferSphere;

	Transform sphereLights;
	void generateSphere();
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
		int index;
		int nrOfLights;
		AnyLight lights[16];
	};

	ModWorld* transModels;
	ModWorld* opaqueModels;
	ConstantBuffer<TheLights> lightsCB;
	void expandGameobjects();
	void expandTransModels();
	void expandOpaqueModels();
	void renderToTexture(float* color);
	void deferredRender();
};

#endif // !GAMEOBJECTHANDLER_H
