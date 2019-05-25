#pragma once
#include "Shader.h"
#include "GBuffer.h"
#include "Structs.h"
#include "KeyFrame.h"
class DeferredShader :
	public Shader
{
public:
	DeferredShader();
	virtual~DeferredShader();
	bool initialize(int height, int width, float nearPlane, float farPlane);
	void setWorld(const DirectX::XMMATRIX& world);
	void setRepeat(const DirectX::XMFLOAT4& repeat);
	void setViewProj(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMFLOAT4 camPos);
	void setSkeleton(bool hasSkeleton);
	void setJointData(std::vector<DirectX::XMMATRIX>& jointTransforms);
	//uses the rendershader in Shader
	void setMaskColor(const DirectX::XMFLOAT4& color);
	void setCamPosToMatricesPerFrame(DirectX::XMFLOAT3 campos);
	void setCBuffers();
	void resetRenderTargets();
	void resetCB();
	void prepDeferredRendering(float* color);
	void prepGBuffer(float* color);
	void prepForLight();
	GBuffer gBuffer;
private:
	ID3D11BlendState* blendState;
	ID3D11RasterizerState* rasState;
	ConstantBuffer<WorldMatrix> worldCB;
	ConstantBuffer<PerFrameMatrices> perFrameCB;
	ConstantBuffer<skinningData> jointCB;
	struct Repeat
	{
		DirectX::XMFLOAT4 texRepeat;
		DirectX::XMFLOAT4 colorMask;
	};
	ConstantBuffer<Repeat> repeat;
};

