#ifndef FullscreenQuad_H
#define FullscreenQuad_H
#include <d3d11.h>
#include <DirectXMath.h>
class FullscreenQuad
{
private:
	struct VertexType
	{
		float x, y, z;
		float u, v;
	};

public:
	FullscreenQuad();
	//FullscreenQuad(const FullscreenQuad&);
	~FullscreenQuad();

	bool Initialize(ID3D11Device* device, int width, int height);
	void Shutdown();
	void insertBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* vertexBuffer;
	int vertexCount;

	bool InitializeBuffers(ID3D11Device* device, int width, int height);
	void ShutdownBuffers();
	void insertBuffer(ID3D11DeviceContext* deviceContext);

};
#endif