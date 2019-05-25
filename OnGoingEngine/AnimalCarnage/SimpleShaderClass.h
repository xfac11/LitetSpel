

#ifndef SIMPLESHADER_H
#define SIMPLESHADER_H
#include"Shader.h"
#include"ConstantBuffer.h"
#include"Structs.h"

class SimpleShader :
	public Shader

{
public:
	SimpleShader();
	virtual~SimpleShader();
	bool initialize();
	void setWorld(const DirectX::XMMATRIX& world);
	void setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 camPos);
	//uses the rendershader in Shader
	void setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos);
	void setCBuffers();
	virtual void renderShader(int vertexCount, int indexCount);
private:
	ID3D11BlendState* blendState;
	ID3D11RasterizerState* rasState;
	ConstantBuffer<WorldMatrix> worldCB;
	ConstantBuffer<PerFrameMatrices> perFrameCB;

	void shutdown();
};

#endif // !SIMPLESHADER_H