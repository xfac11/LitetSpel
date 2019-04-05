#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
//#include <d3d11.h>
//#include <directxmath.h>
#include <vector>
//#include "Texture.h"
#include "Loader.h"
#include "DeferedShader.h"
class GameObject
{
private:
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


	ID3D11Buffer *vertexBuffer;
	ID3D11SamplerState* SamplerState;

	void calculateModelVectors();     //NORMAL MAPS
	void calculateTangentBinormal(NM_Vertex vertex1, NM_Vertex vertex2, NM_Vertex vertex3, DirectX::XMFLOAT3& tangent, DirectX::XMFLOAT3& binormal, DirectX::XMFLOAT3& normal);
	void calculateNormal(DirectX::XMFLOAT3 tangent, DirectX::XMFLOAT3 binormal, DirectX::XMFLOAT3 & normal);     //NORMAL MAPS

public:
	GameObject();
	~GameObject();


	void setWorld();
	DirectX::XMFLOAT4X4 getWorld();
	void Render(DeferedShader & shader, ID3D11DeviceContext* deviceContext);

	void shutdown();
};
#endif