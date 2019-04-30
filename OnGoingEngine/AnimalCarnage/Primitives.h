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
class Primitives :public Transform
{
private:
	std::vector<VertexObject> vertices;
	std::vector<DWORD> indices;

	VertexBuffer<VertexObject> vertexBuffer;
	IndexBuffer indicesbuffer;
	bool CreateQuad();
	UINT indice;
public:
	//functions
	Primitives();

	void initialize(int shapes);
	~Primitives();
	void draw(SimpleShader* shader);
};


#endif // !PRIMITIVES_H
