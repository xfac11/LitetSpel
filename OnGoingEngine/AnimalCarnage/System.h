#ifndef SYSTEM_H
#define SYSTEM_H
#include <math.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include"GraphicsDevice.h"
#include"ForwardShader.h"
#include"GameObject.h"
#include"Camera.h"
#include"Keyboard.h"
#include"Mouse.h"
#include"GameObjectHandler.h"
#include "GraphicsDevice.h"
#include "ForwardShader.h"
#include "GameObject.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "InputHandler.h"
#include <string>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "State.h"

using namespace DirectX;
//using namespace DirectX::SimpleMath;

#define HEIGHT 1080
#define WIDTH 1920

//using Microsoft::WRL::ComPtr;

enum GameState {
	MAINMENU
};

class System
{
private:


	enum Direction  // for the camera
	{
		Neutral = 0,
		Positive = 1,
		Negative = -1
	};

	LPCSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;
	MSG msg;
	int nCMDShow;
	//Statemachine* statemachine
	//Graphics* graphics;
	
	static GraphicsDevice* theGraphicDevice;
	static std::vector<State*> states;
	static GameState currentState;

	
	GameObject* obj;
	GameObject* obj2;
	GameObject* playerOne;
	GameObject* playerTwo;
	int currentInput; //imgui, controlls input
	ForwardShader* theForwardShader;
	Camera* theCamera;
	Direction forward; //have these in a own class?
	Direction left_right;
	Direction up_down;
	GameObjectHandler handler;
	void move(Direction forward, Direction left_right, Direction up_down, bool flyMode, int mouseX, int mouseY);
	
	
	InputHandler* playerInputs;

	//imgui variables
	DirectX::XMFLOAT3 camPos;
	DirectX::XMFLOAT3 camRot;
	bool guiCam;
	bool collide;

	bool mouseShow;
	bool mouseSwitch;
	bool flySwitch;
	bool moveScreen;
	bool isPressed;

	void initImgui();
	void renderImgui();
	void update(float deltaTime);
	void render();
	void resetShaders();

	void reset();
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
	static ID3D11Device*& getDevice();
	static ID3D11DeviceContext*& getDeviceContext();

	static Keyboard* theKeyboard;
	static Mouse* theMouse;

};
#endif