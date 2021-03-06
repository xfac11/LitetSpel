#ifndef PARTICLE_SHADER_H
#define PARTICLE_SHADER_H

#include "Shader.h"
#include "Structs.h"
#include "ConstantBuffer.h"

class ParticleShader : public Shader
{
private:
	struct ParticleConfig
	{
		float size;
	};
	ConstantBuffer<PerFrameMatrices> perFrameCB;
	ConstantBuffer<ParticleCamera> cameraCB;
	ConstantBuffer<ParticleConfig> particleConfigCB;
public:
	ParticleShader();
	virtual ~ParticleShader();

	bool initialize();
	void setSize(float size);
	void setWorld(const DirectX::XMMATRIX& world);
	void setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 campos);
	void setCamera(DirectX::XMFLOAT3 camUp);
	void setCBuffers();
};

#endif // !PARTICLE_SHADER_H
