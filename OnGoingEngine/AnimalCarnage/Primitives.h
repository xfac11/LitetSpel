#ifndef PRIMITIVES_H
#define PRIMITIVES_H
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "SimpleShaderClass.h"
#include "Transform.h"
struct VertexObject
{
	float x, y, z;
	float cx, cy, cz;
};
class Primitives
{
private:
	std::vector<VertexObject> vertices;
	std::vector<DWORD> indices;

	VertexBuffer<VertexObject> vertexBuffer;
	IndexBuffer indicesbuffer;
	bool CreateQuad();
	UINT indice;
	XMMATRIX* world;
public:
	//functions
	Primitives();
	void setWorld(XMMATRIX* xmWorld) { this->world = xmWorld; }
	void initialize(int shapes);
	~Primitives();
	void draw(SimpleShader* shader);
};


#endif // !PRIMITIVES_H
