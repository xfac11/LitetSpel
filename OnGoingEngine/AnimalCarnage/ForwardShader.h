#ifndef FORWARDSHADER_H
#define FORWARDSHADER_H
#include"Shader.h"
#include"ConstantBuffer.h"
#include"Structs.h"
class ForwardShader:
	public Shader

{
public:
	ForwardShader();
	virtual~ForwardShader();
	bool initialize();
	void setWorld(const DirectX::XMMATRIX& world);
	void setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 camPos);
	//uses the rendershader in Shader
	void setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos);
	void setCBuffers();
private:
	ID3D11BlendState* blendState;
	ID3D11RasterizerState* rasState;
	ConstantBuffer<WorldMatrix> worldCB;
	ConstantBuffer<PerFrameMatrices> perFrameCB;

	void shutdown();
};
#endif // !FORWARDSHADER_H
