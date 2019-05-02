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
#include "ShaderManager.h"
#include "Skybox.h"
#include "SoundManager.h"
//Test
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionShapes\btBoxShape.h"
#include "Physics.h"
#include "DEBUG_DRAW.h"

using namespace DirectX;
//using namespace DirectX::SimpleMath;

#define HEIGHT 1080
#define WIDTH 1920

//using Microsoft::WRL::ComPtr;

enum GameState {
	MAINMENU,
	GUNGAME
};

class System
{
private:
	LPCSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;
	MSG msg;
	int nCMDShow;
	
	static std::vector<State*> states;
	static GameState currentState;
	static CommonStates* commonStates;
	static SpriteBatch* spriteBatch;
	static SpriteFont* fontComicSans;
	static SpriteFont* fontArial;
	static Physics* physices;
	static DEBUG_DRAW* debugDraw;
	static SoundManager* soundManager;

	//ForwardShader* theForwardShader;
	Camera* theCamera;

	//IMGUI VARIABLES
	//DirectX::XMFLOAT3 camPos;
	//DirectX::XMFLOAT3 camRot;
	//bool guiCam;
	//bool freezeCheck;
	//DirectX::XMFLOAT3 cullingPos;
	//bool collide;
	//int currentInput; //imgui, controlls input

	bool mouseShow;
	bool mouseSwitch;
	bool moveScreen;

	void initImgui();
	//void renderImgui();
	void update(float deltaTime);
	void render();
	void resetShaders();

	void reset();
	static LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND InitWindow(HINSTANCE hInstance, float height, float width);
	void change(bool & theSwitch);

	//btCollisionConfiguration* collisionConfig; //Test
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
	
	static Skybox* skybox;

	static ShaderManager* shaderManager;
	static System* fusk;

	static Keyboard* theKeyboard;
	static Mouse* theMouse;
	static GamePad* theGamePad;
	static GamePad::ButtonStateTracker* theTracker;

	static GameObjectHandler* handler;
	static GraphicsDevice* theGraphicDevice;


	static ModelLoader* theModelLoader;
	static SpriteBatch* getSpriteBatch();
	static CommonStates* getCommonStates();
	static SoundManager* getSoundManager();
	static SpriteFont* getFontComicSans();
	static SpriteFont* getFontArial();
	static Physics* getphysices();
	static DEBUG_DRAW* getDebugDraw();
	static void closeWindow();
	static void setState(GameState state);


};
#endif