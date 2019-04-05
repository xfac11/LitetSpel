#ifndef GBUFFER_H
#define GBUFFER_H
#include <d3d11.h>
#include <directxmath.h>
const int GBUFFERCAP = 4;
class GBuffer
{
public:
	GBuffer();
	~GBuffer();
	bool initialize(ID3D11Device* device, int height, int width, float nearPlane, float farPlane);
	bool setRenderTargets(ID3D11Device*& device, ID3D11DeviceContext* deviceContext);
	ID3D11RenderTargetView*& getRenView(int id);
	void shutDown();
	void clear(ID3D11DeviceContext *deviceContext, float color[4]);
	void setShaderResViews(ID3D11DeviceContext* deviceContext);
	ID3D11DepthStencilView*& getDepthStcView();
	ID3D11ShaderResourceView* getShadResView(int id);
	ID3D11Texture2D* getTexture(int id);
private:
	ID3D11Texture2D *texTures[GBUFFERCAP];
	ID3D11RenderTargetView *renderTars[GBUFFERCAP];
	ID3D11ShaderResourceView *shaderResViews[GBUFFERCAP];
	ID3D11Texture2D *depthBuffer;
	ID3D11DepthStencilView *depthStencView;
	D3D11_VIEWPORT viewP;
	ID3D11ShaderResourceView *depthSource;
	int height;
	int width;
};
#endif // !GBUFFER_H