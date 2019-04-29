#ifndef PRIMITIVES_H
#define PRIMITIVES_H
#include "Transform.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <iostream>
#include "System.h"
using namespace std;
enum SHAPES {
	QUAD
};
struct Vertex
{
	float x, y, z;
	float cx, cy, cz;
};
class Primitives : public Transform
{
private:
	UINT indice;

	ID3D11Buffer* VerticesBuffer;
	ID3D11Buffer* IndicesBuffer;
	bool CreateQuad();
public:
	//functions
	Primitives(SHAPES shapes= QUAD);
	~Primitives();
	ID3D11Buffer** GetVerticesBuffer() { return &this->VerticesBuffer; }
	ID3D11Buffer* GetIndicesBuffer() { return this->IndicesBuffer; }
	UINT GetIndices() { return this->indice; }
};


#endif // !PRIMITIVES_H
