#ifndef SYSTEM_H
#define SYSTEM_H
#include <math.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include"GraphicsDevice.h"
#include <string>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
using namespace DirectX;
//using namespace DirectX::SimpleMath;

#define HEIGHT 768.0f
#define WIDTH 768.0f
//using Microsoft::WRL::ComPtr;
class System
{
private:
	LPCSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;
	MSG msg;
	int nCMDShow;
	
	//Statemachine* statemachine
	//Graphics* graphics;
	//static Keyboard* theKeyboard;
	//static Mouse* theMouse;
	static GraphicsDevice* theGraphicDevice;
	

	bool mouseSwitch;
	bool flySwitch;
	bool moveScreen;

	//Direction forward; //have these in a own class?
	//Direction left_right;
	//Direction up_down;


	//temp pls remove this later
	bool move1;
	bool move2;

	void initImgui();
	void update(float deltaTime);
	void render();

	static LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND InitWindow(HINSTANCE hInstance, float height, float width);
	void change(bool & theSwitch);
public:

	System(HINSTANCE hInstance, LPCSTR name, int nCmdShow);
	~System();
	//System(const System& obj);
	bool initialize();
	void run();
	void shutDown();
	WPARAM getMsgWParam();
	static ID3D11Device* getDevice();
	static ID3D11DeviceContext* getDeviceContext();

};
#endif