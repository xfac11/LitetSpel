#ifndef MODEL_H
#define MODEL_H
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"ForwardShader.h"
#include"Texture.h"
#include"Structs.h"
#include <memory>
#include <iostream>
using namespace std;
enum Opacity
{
	Opaque,
	Transparent
};
class Model
{
public:
	
	Model();
	~Model();
	int getOpacity();
	void setGotSkeleton(bool gotSkltn);
	Shader* getShader();
	void setShader(Shader* theShader);
	void setRepeat(float x, float y);
	void setMaskColor(DirectX::XMFLOAT4 colorMask);
	DirectX::XMFLOAT4 &getMaskColor();

	DirectX::XMFLOAT4 &getRepeat();
	void setTexture(std::string file, int mipLevels=-1);

	void setGlowMap(std::string file);
	void setMask(std::string file);
	void setMesh(std::vector<Vertex3D> aMesh,DWORD *indices, int numberOfIndices);
	void setSampler();
	void setSampler(D3D11_TEXTURE_ADDRESS_MODE type, D3D11_FILTER filter, D3D11_COMPARISON_FUNC comFunc);
	void drawOnlyVertex();
	void draw();
	void SetTexture(shared_ptr<Texture>);// { texture = t; }
public:


private:
	shared_ptr<Texture> texture;// 
	//skeleton
	bool gotSkeleton;
	
	//std::vector<Joint> skeleton;
	//Animation anims;
	//float timePassed;

	DirectX::XMFLOAT4 colorMask;
	DirectX::XMFLOAT4 repeatXY;
	std::vector<Vertex3D> mesh;
	VertexBuffer<Vertex3D> vertexBuffer;
	IndexBuffer indexBuffer;
	ID3D11SamplerState* SamplerState;
	//mask
	Texture* normalMap;
	Texture* glowMap;
	Texture* mask;
	bool hasGlowMap;
	bool hasMask;
	int vertexCount;
	
	//Joint jointHierarchy
	//int3 jointIDs
	//float3 weights

	Opacity type;
	Shader * theShader;//pointer to the shader it uses. Comes from system or state. Do not delete
};
#endif // !MODEL_H
