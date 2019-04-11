#ifndef MODEL_H
#define MODEL_H
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"ForwardShader.h"
#include"Texture.h"
#include"Structs.h"
class Model
{
public:
	Model();
	~Model();
	void setShader(Shader* theShader);
	Shader* getShader();
	void setTexture(std::string file);
	void setMesh(std::vector<Vertex3D> aMesh,DWORD *indices, int numberOfIndices);
	void setSampler();
	void setSampler(D3D11_TEXTURE_ADDRESS_MODE type, D3D11_FILTER filter, D3D11_COMPARISON_FUNC comFunc);
	void draw();
private:
	std::vector<Vertex3D> mesh;
	VertexBuffer<Vertex3D> vertexBuffer;
	IndexBuffer indexBuffer;
	ID3D11SamplerState* SamplerState;
	Texture* texture;
	Texture* normalMap;
	int vertexCount;

	Shader * theShader;//pointer to the shader it uses. Comes from system or state. Do not delete
};
#endif // !MODEL_H
