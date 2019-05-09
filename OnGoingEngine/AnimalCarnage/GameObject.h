#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include"Model.h"
#include"Transform.h"
#include"ConstantBuffer.h"
#include"Geometry.h"
#include "btBulletDynamicsCommon.h"
#include "Primitives.h"
//#include "Player.h"

class GameObject: public Transform//arv
{
public:
	GameObject();
	GameObject(Shader * shader);
	~GameObject();

	int getNrOfModels();
	Model*& getModel(int id);
	Model**& getTheModelPtr();

	void setHalfSize(float halfSize[3], float posOffset[3]);

	void calcAABB(std::vector<Vertex3D> mesh);

	void addModel(std::vector<Vertex3D> mesh, DWORD *indices, int numberOfIndices);
	void setMesh(std::vector<Vertex3D> mesh, DWORD *indices, int numberOfIndices, int id);
	void setTexture(std::string file, int id);
	void draw();
	AABB getCollisionBox();
	Primitives * CollisionShape;
	btVector3 positionOffset;
	/*DatForaObject data;*/
private:
	Model* *theModel;
	int cap;

	bool hasLoadedAABB;

	AABB colBox;
	//ConstantBuffer<WorldMatrix> worldConstBuffer;
	//Hitbox theHitbox; a model with just the index and vertices and a color. no textures etc
	int nrOfModels;
};
#endif // !GAMEOBJECT_H
