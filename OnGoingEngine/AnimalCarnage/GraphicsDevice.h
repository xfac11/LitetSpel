#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H
#include <d3d11.h>
#include <directxmath.h>
#include<wrl/client.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using Microsoft::WRL::ComPtr;

class GraphicsDevice
{
public:
	GraphicsDevice();
	~GraphicsDevice();
	void* operator new(size_t i) //test
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
	void beginScene(float color[4]);
	void presentScene();
	bool initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void shutDown();
	DirectX::XMMATRIX getProj();
	static ID3D11Device*& getDevice();
	static ID3D11DeviceContext*& getDeviceContext();

	void turnOnZ();
	void turnOffZ();
	void setRasterState();

	void setBlendState();
	void setBackBuffer();
	void setBackBuffer(ID3D11DepthStencilView*& view);

private:

	bool vSync_enabled;
	float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	IDXGISwapChain* swapChain;
	static ID3D11Device *device;
	static ID3D11DeviceContext *deviceContext;
	D3D11_VIEWPORT vp;
	ID3D11Debug* debug;

	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* disableDepthStencilState;
	ID3D11DepthStencilView* depthStencilView;


	ID3D11RasterizerState* rasterState;
	DirectX::XMMATRIX projectionMatrix;


	//particles use this
	ID3D11BlendState* alphaEnableBlendingState;
	ID3D11BlendState* alphaDisableBlendingState;




};
#endif // !GRAPHICSDEVICE_H