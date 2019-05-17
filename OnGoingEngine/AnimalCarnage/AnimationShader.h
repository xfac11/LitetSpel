#ifndef ANIMATIONSHADER_H
#define ANIMATIONSHADER_H
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Structs.h"
#include "KeyFrame.h"
class AnimationShader :
	public Shader
{
private:
	ID3D11BlendState* blendState;
	ID3D11RasterizerState* rasState;
	ConstantBuffer<WorldMatrix> worldCB;
	ConstantBuffer<PerFrameMatrices> perFrameCB;
	ConstantBuffer<skinningData> jointCB;

	void shutdown();
public:
	AnimationShader();
	virtual~AnimationShader();
	bool initialize();
	void setWorld(DirectX::XMMATRIX world);
	void setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 camPos);
	void setJointData(std::vector<KeyFrame> keyframes);
	//uses the rendershader in Shader
	void setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos);
	void setCBuffers();
};
#endif // !ANIMATIONSHADER_H
