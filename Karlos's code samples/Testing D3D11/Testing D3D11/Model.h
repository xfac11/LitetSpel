#ifndef MODEL_H
#define MODEL_H
#include <d3d11.h>
#include <directxmath.h>
#include <vector>
#include "CubeHandler.h"
#include "QuadHandler.h"
#include "Texture.h"
#include "Loader.h"
//#include "ColorShader.h" //defered
#include "DeferedShader.h"

class Model
{
public:

	Model();
	~Model();
	//This before setVertexBuffer
	
	bool addQuads(DirectX::XMFLOAT3 pos, float width, float height, float depth, int face);
	bool addCube(DirectX::XMFLOAT3 pos, float width, float height, float depth);
	bool addTri(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3);
	void shutdown();
	bool insertCubesInVec();
	void moveCube(int id);
	bool createTheVertexBuffer(ID3D11Device *& gDevice);
	void setVertexBandTexture(ID3D11DeviceContext *& gDeviceContext);
	int getVertexCount()const;
	void setTheTexture(ID3D11Device*& gDevice, ID3D11DeviceContext *&gDeviceContext,std::string filename, std::string normalFileName );
	void setSampler(ID3D11Device*& gDevice);

	void loadOBJ(char* file, char* normal, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	//void draw(ColorShader &shader, ID3D11DeviceContext * deviceContext); //forawrd
	void draw(DeferedShader &shader, ID3D11DeviceContext * deviceContext);
	
	DirectX::XMFLOAT4X4 getId();
	void setWorld(DirectX::XMMATRIX mtrx);
	void setWorld();
	void setPosition(float x, float y, float z);
	void move(float x, float y, float z);
	void rotate(DirectX::XMVECTOR axis, float angle);
	float rot;
	float moveM;
	void billboard(DirectX::XMFLOAT3 camPos);
	DirectX::XMFLOAT3 getPosition()const;


	void setDistanceToCam(DirectX::XMFLOAT3 camPos);
	float getDistanceToCam();

private:

	ID3D11Buffer *vertexBuffer;
	//ID3D11Buffer *indexBuffer;
	//ID3D11Buffer *constantBuffer;
	ID3D11SamplerState* SamplerState;

	std::vector<Vertex3D> body;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 Rotation;
	DirectX::XMFLOAT4X4 Scale;
	DirectX::XMFLOAT4X4 Translation;
	DirectX::XMFLOAT3 position;

	Texture texture;
	Texture normal;
	Loader load;
	int vertexCount;
	//int indexCount;
	QuadHandler quads;
	CubeHandler cubes;


	float distanceToCam;


	void calculateModelVectors();     //NORMAL MAPS
	void calculateTangentBinormal(NM_Vertex vertex1, NM_Vertex vertex2, NM_Vertex vertex3, DirectX::XMFLOAT3& tangent, DirectX::XMFLOAT3& binormal, DirectX::XMFLOAT3& normal);
	void calculateNormal(DirectX::XMFLOAT3 tangent, DirectX::XMFLOAT3 binormal, DirectX::XMFLOAT3 & normal);     //NORMAL MAPS



};
#endif // !MODEL_H