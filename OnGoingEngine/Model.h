#ifndef MODEL_H
#define MODEL_H
#include"Shader.h"
#include"Texture.h"
#include"Structs.h"
class Model
{
public:
	Model();
	~Model();


private:
	std::vector<Vertex3D> mesh;
	ID3D11Buffer *vertexBuffer;
	//ID3D11Buffer *indexBuffer;
	//ID3D11Buffer *constantBuffer;
	ID3D11SamplerState* SamplerState;
	Texture texture;
	Texture normal;
	int vertexCount;
};
#endif // !MODEL_H
