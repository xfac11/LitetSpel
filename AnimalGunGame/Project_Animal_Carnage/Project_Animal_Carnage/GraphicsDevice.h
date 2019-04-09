#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H
#include <d3d11.h>
#include <directxmath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
class GraphicsDevice
{
public:
	GraphicsDevice();
	~GraphicsDevice();

	bool initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
private:
	bool vSync_enabled;

	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
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
