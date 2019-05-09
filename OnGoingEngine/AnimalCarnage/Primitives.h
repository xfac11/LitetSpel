#ifndef PRIMITIVES_H
#define PRIMITIVES_H
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "SimpleShaderClass.h"
#include "Transform.h"
#include <btBulletDynamicsCommon.h>


struct VertexObject
{
	float x, y, z;
	float cx, cy, cz;
};
class Primitives
{
private:
	VertexBuffer<VertexObject> vertexBuffer;
	IndexBuffer indicesbuffer;
	bool CreateBox(btVector3 position,btVector3 size);
	UINT indices;
	XMMATRIX* world;
public:
	//functions
	Primitives();
	void SetWorld(XMMATRIX* xmWorld) { this->world = xmWorld; }
	void Initialize(int shapes, btVector3 position,btVector3 size);
	~Primitives();
	void Draw(SimpleShader* shader);
};
struct DatForaObject
{
	btRigidBody* rigidbody = nullptr;
	Primitives* primitiv = nullptr;
	float health = 0;
	DatForaObject():rigidbody(nullptr), primitiv(nullptr),health(0){}
};


#endif // !PRIMITIVES_H
