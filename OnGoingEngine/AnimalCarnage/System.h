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
#include "GamePad.h"
//#include "InputHandler.h"
#include <string>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "State.h"
#include "CommonStates.h"
#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "ModelLoader.h"
#include"ShaderManager.h"

using namespace DirectX;
//using namespace DirectX::SimpleMath;

#define HEIGHT 1080
#define WIDTH 1920

//using Microsoft::WRL::ComPtr;

//rumble
struct RumbleTemp
{
	float rumbleClock;
	float rumbleTime;
	DirectX::XMFLOAT2 rumble;
};

enum GameState {
	MAINMENU,
	GUNGAME
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
	static CommonStates* commonStates;
	static SpriteBatch* spriteBatch;
	static SpriteFont* fontComicSans;
	
	GameObject* obj;
	GameObject* obj2;
	//GameObject* playerOne;
	//GameObject* playerTwo;
	GameObject* players[4];
	int currentInput; //imgui, controlls input
	//ForwardShader* theForwardShader;
	Camera* theCamera;
	//Direction forward; 
	//Direction left_right;
	//Direction up_down;
	GameObjectHandler handler;
	
	//InputHandler* playerInputs;
	//DirectX::GamePad 	

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
	static void updateRumble(float deltaTime, int id);
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

	static ShaderManager* shaderManager;
	static System* fusk;

	static Keyboard* theKeyboard;
	static Mouse* theMouse;
	static GamePad* theGamePad;
	static GamePad::ButtonStateTracker* theTracker;
	static RumbleTemp theRumble[4];

	static ModelLoader* theModelLoader;
	static SpriteBatch* getSpriteBatch();
	static CommonStates* getCommonStates();
	static SpriteFont* getFontComicSans();

	static void closeWindow();
	static void setState(GameState state);
};
#endif