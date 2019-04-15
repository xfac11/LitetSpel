#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include"Model.h"
#include"Transform.h"
#include"ConstantBuffer.h"
class GameObject
{
public:
	GameObject();
	GameObject(Shader * shader);
	~GameObject();
	void setPosition(float x, float y, float z);
	void setRotation();
	void setScale(float x, float y, float z);
	void move(float x, float y, float z);


	DirectX::XMFLOAT3 getPosition();

	int getNrOfModels();
	Model& getModel(int id);
	void setMesh(std::vector<Vertex3D> mesh, DWORD *indices, int numberOfIndices, int id);
	void setTexture(std::string file, int id);
	void draw();
private:
	Model *theModel;
	int cap;
	//ConstantBuffer<WorldMatrix> worldConstBuffer;
	//Hitbox theHitbox; a model with just the index and vertices and a color. no textures etc
	Transform theTransforms;
	int nrOfModels;
};
#endif // !GAMEOBJECT_H
