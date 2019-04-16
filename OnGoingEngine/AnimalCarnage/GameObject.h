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
	DirectX::XMFLOAT3 getScale();


	void setMesh(std::vector<Vertex3D> mesh, DWORD *indices, int numberOfIndices);
	void setTexture(std::string file);
	void draw();
private:
	Model *theModel;
	//ConstantBuffer<WorldMatrix> worldConstBuffer;
	//Hitbox theHitbox; a model with just the index and vertices and a color. no textures etc
	Transform theTransforms;
};
#endif // !GAMEOBJECT_H
