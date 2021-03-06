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
	//void addObject(char *file);
	GameObject& getObject(int id);
	void sortBackToFront();
	void draw(float deltaTime, bool isPaused, std::vector<float> & playerSpeed, std::vector<string> & playerName, std::vector<bool> & playerLoop);
	void initialize();
	void addLight(float pos[4], float dir[4], float color[4]);
	void setSkyboxTexture(std::string file);
	void setSunDir(DirectX::XMFLOAT3 dir);
	void setLightPos(int id, btVector3 pos);
	void reset();
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
	string animName;

	std::vector<float> playerAnimTimer;

	//int nrOfAnimsObj
	struct ModWorld
	{
		DirectX::XMMATRIX* worldPtr;
		shared_ptr<Model> modelPtr;
		GameObject* selfPtr;
	};

	
	struct LightVertex
	{
		float position[3];
		float uv[2];
	};
	std::vector<LightVertex> quad;
	VertexBuffer<LightVertex> vertexBufferQuad;
	//optimise
	float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };

	float color[4] = {
		0,0,0,1.0f
	};
	DirectX::XMMATRIX *lightSphereWorld;
	DirectX::XMMATRIX lightView;
	DirectX::XMVECTOR lightDirView = DirectX::XMVectorSet(0, 0, 0, 1);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMVECTOR lightCamPos;//for shadow

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
