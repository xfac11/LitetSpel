#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H
#include "SimpleShaderClass.h"
#include "Primitives.h"
#include "ConstantBuffer.h"

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};
class DEBUG_DRAW
{
private:
	SimpleShader* shader;

	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ConstantBuffer<MatrixBufferType> matrixBuffer;

public:
	DEBUG_DRAW();
	~DEBUG_DRAW();

	void Draw(Primitives* shape);
};

#endif
