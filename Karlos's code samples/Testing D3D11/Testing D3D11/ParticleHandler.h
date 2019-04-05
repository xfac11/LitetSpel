//#ifndef PARTICLEHANDLER_H
//#define PARTICLEHANDLER_H
//#include "Particle.h"
//class ParticleHandler
//{
//private:
//	Particle* particles;
//	Vertex3D* vertices;
//
//
//	int vertexCount;
//	int indexCount;
//	ID3D11Buffer *vertexBuffer;
//	ID3D11Buffer *indexBuffer;
//	ID3D11SamplerState* SamplerState;
//	Texture theTexture;
//
//	DirectX::XMFLOAT3 particleDeviation;
//	float particleVelocity;
//	float particleVelocityVariation;
//	float particleSize;
//	float particlesPerSecond;
//	int maxParticles;
//	int currentParticleCount;
//	float accumulatedTime;
//
//	bool InitializeParticleSystem();
//	bool InitializeBuffers(ID3D11Device* device);
//
//	void EmitParticles(float frameTime);
//	void UpdateParticles(float frameTime);
//	void KillParticles();
//
//	bool LoadTexture(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, std::string filename);
//	bool UpdateBuffers(ID3D11DeviceContext* deviceContext);
//
//public:
//	ParticleHandler();
//	~ParticleHandler();
//
//	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string textureName);
//	void Shutdown();
//	bool Frame(float frameTime, ID3D11DeviceContext* deviceContext);
//	void Render(ColorShader & shader, ID3D11DeviceContext* deviceContext);
//
//	void setWorld();
//	DirectX::XMFLOAT4X4 getWorld(int id);
//	int getNrOfParticles();
//	void billboard(DirectX::XMFLOAT3 camPos);
//	//void setPosition(float x, float y, float z);
//	void setSampler(ID3D11Device *& gDevice);
//};
//#endif