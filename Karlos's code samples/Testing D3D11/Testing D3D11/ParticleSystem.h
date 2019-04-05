#ifndef PARTICLESYSTEM
#define PARTICLESYSTEM
#include <d3d11.h>
#include <directxmath.h>
#include <string>
#include <vector>
#include "Texture.h"
#include "Vertex3D.h"
//#include "colorShader.h"
#include "DeferedShader.h"
class ParticleSystem
{
private:
	struct ParticleType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 rgb;
		float velocity;
		bool active;
	};

	//struct VertexType
	//{
	//	DirectX::XMFLOAT3 position;
	//	DirectX::XMFLOAT2 texture;
	//	DirectX::XMFLOAT4 color;
	//};
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 Rotation;
	DirectX::XMFLOAT4X4 Scale;
	DirectX::XMFLOAT4X4 Translation;
	DirectX::XMFLOAT3 position;


	DirectX::XMFLOAT3 particleDeviation;
	float particleVelocity;
	float particleVelocityVariation;
	float particleSize;
	float particlesPerSecond;
	int maxParticles;

	int currentParticleCount;
	float accumulatedTime;

	Texture theTexture;
	Texture normal;
	ParticleType* particleList;

	int vertexCount;
	int indexCount;
	//Vertex3D* vertices;
	std::vector<Vertex3D> body;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;
	ID3D11SamplerState* SamplerState;

	bool LoadTexture(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, std::string filename, std::string normalFileName);
	
	bool InitializeParticleSystem();

	bool InitializeBuffers(ID3D11Device* device);

	void EmitParticles(float frameTime);
	void UpdateParticles(float frameTime);
	void KillParticles();

	bool UpdateBuffers(ID3D11DeviceContext* deviceContext);

	void calculateModelVectors();     //NORMAL MAPS
	void calculateTangentBinormal(NM_Vertex vertex1, NM_Vertex vertex2, NM_Vertex vertex3, DirectX::XMFLOAT3& tangent, DirectX::XMFLOAT3& binormal, DirectX::XMFLOAT3& normal);
	void calculateNormal(DirectX::XMFLOAT3 tangent, DirectX::XMFLOAT3 binormal, DirectX::XMFLOAT3 & normal);     //NORMAL MAPS


public:
	ParticleSystem();
	~ParticleSystem();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string textureName, std::string normalFileName);
	void Shutdown();
	bool Frame(float frameTime, ID3D11DeviceContext* deviceContext);
	//void Render(ColorShader & shader, ID3D11DeviceContext* deviceContext);
	void draw(DeferedShader & shader, ID3D11DeviceContext* deviceContext);

	//int GetIndexCount();
	void setWorld();
	DirectX::XMFLOAT4X4 getWorld();
	void billboard( DirectX::XMFLOAT3 camPos);
	void setPosition(float x, float y, float z );
	void setSampler(ID3D11Device *& gDevice);
};
#endif // !PARTICLESYSTEM

