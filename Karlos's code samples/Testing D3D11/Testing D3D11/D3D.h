#ifndef D3D_H
#define D3D_H
#include <d3d11.h>
#include <directxmath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
__declspec(align(16)) class D3D
{

private:
	bool vSync_enabled;
	int videoCardMemory;
	char videoCardDescription[128];
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* disableDepthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT vp;
	ID3D11RasterizerState* rasterState; 
	DirectX::XMMATRIX projectionMatrix;
	DirectX::XMMATRIX worldMatrix;

	//particles use this
	ID3D11BlendState* alphaEnableBlendingState;
	ID3D11BlendState* alphaDisableBlendingState;

	ID3D11Debug* debug;

	float dist;
	float gIncrement;

public:
	D3D();
	~D3D();
	void* operator new(size_t i) //test
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
	void setIncrement(float g);
	bool initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();
	void BeginScene(float color[4]);
	void EndScene();

	void setBackBuffer();
	void setBackBuffer(ID3D11DepthStencilView* view);
	void turnOnZ();
	void turnOffZ();
	void resetViewPort();
	void EnableAlphaBlending();
	void DisableAlphaBlending();

	ID3D11Device*& GetDevice();
	ID3D11DeviceContext*& GetDeviceContext();
	DirectX::XMMATRIX& GetProjectionMatrix();
	DirectX::XMMATRIX& GetWorldMatrix();

	int & GetVideoCardInfo(char* cardName);//(, int& memory);
};
#endif