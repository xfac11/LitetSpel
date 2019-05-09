#include"System.h"
#include "MainMenu.h"
#include "GunGameState.h"

//Keyboard* System::theKeyboard = 0;//for static
GraphicsDevice* System::theGraphicDevice = nullptr;
Mouse* System::theMouse = nullptr;
Keyboard* System::theKeyboard = nullptr;
GamePad* System::theGamePad = nullptr;
GamePad::ButtonStateTracker* System::theTracker = nullptr;
ModelLoader* System::theModelLoader = nullptr;
std::vector<State*> System::states = std::vector<State*>();
GameState System::currentState = GameState::GUNGAME;
System* System::fusk = nullptr;
CommonStates* System::commonStates = nullptr;
SpriteBatch* System::spriteBatch = nullptr;
SpriteFont* System::fontComicSans = nullptr;
SpriteFont* System::fontArial = nullptr;
ShaderManager* System::shaderManager = nullptr;
GameObjectHandler* System::handler = nullptr;
Physics* System::physices = nullptr;
DEBUG_DRAW* System::debugDraw = nullptr;
Skybox* System::skybox = nullptr;
SoundManager* System::soundManager = nullptr;
WindowClient System::theWindow = { HEIGHT,WIDTH };
Camera* System::theCamera = nullptr;

HWND System::InitWindow(HINSTANCE hInstance, float height, float width)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	//wcex.hIcon = NULL;
	//wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//wcex.hbrBackground = NULL;
	wcex.lpszClassName = "Demo";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, (int)width, (int)height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindow(
		"Demo",
		this->applicationName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		this);
	return hwnd;
}
void System::change(bool & theSwitch)
{
	if (theSwitch == true)
		theSwitch = false;
	else
		theSwitch = true;
}

void System::reset()
{
	System::getDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->PSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->IASetInputLayout(nullptr);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lParam);
LRESULT CALLBACK System::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{ //event-handling, event happens to window
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	System *sys;
	if (message == WM_NCCREATE)
	{
		OutputDebugStringA("Window Created.\n");
		sys = (System*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(sys));
	}
	else
		sys = (System*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (message == WM_DESTROY)
		PostQuitMessage(0);
	else if (message == WM_CHAR)
	{
		if (sys != nullptr)
		{
			unsigned char theChar = static_cast<unsigned char>(wParam);
			std::string msg;
			msg.push_back(theChar);
			if (theKeyboard->IsCharsAutoRepeat())
			{
				theKeyboard->OnChar(theChar);
			}
			else
			{
				const bool wasPressed = lParam & 0x40000000;
				if (!wasPressed)
				{
					theKeyboard->OnChar(theChar);

				}
			}
			//OutputDebugStringA(msg.c_str());
			//MessageBox(hWnd, msg.c_str(), "Keyboard Input", MB_OK); //L"", L"", ;
		}
	}
	else if (message == WM_KEYDOWN)
	{
		unsigned char theKey = static_cast<unsigned char>(wParam);
		if (theKeyboard->IsKeysAutoRepeat())
		{
			theKeyboard->OnKeyPressed(theKey);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				theKeyboard->OnKeyPressed(theKey);
			}
		}
	}
	else if (message == WM_KEYUP)
	{
		unsigned char theKey = static_cast<unsigned char>(wParam);
		theKeyboard->OnKeyReleased(theKey);
	}
	else if (message == WM_MOUSEMOVE)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		theMouse->OnMouseMove(x, y);
	}
	else if (message == WM_INPUT)
	{
		UINT dataSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
		if (dataSize > 0)
		{
			//std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize); 
			BYTE* rawdata = new BYTE[dataSize];
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata, &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata);
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					theMouse->OnMouseRAW(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}

			}
			delete[] rawdata;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	else if (message == WM_LBUTTONDOWN)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		theMouse->OnLeftPressed(x, y);
	}
	else if (message == WM_LBUTTONDOWN)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		theMouse->OnRightPressed(x, y);
	}
	//else if (message == WM_MBUTTONDOWN)
	//{
	//	int x = LOWORD(lParam);
	//	int y = HIWORD(lParam);
	//	theMouse->OnMiddlePressed(x, y);
	//}
	//else if (message == WM_LBUTTONUP)
	//{
	//	int x = LOWORD(lParam);
	//	int y = HIWORD(lParam);
	//	theMouse->OnLeftReleased(x, y);
	//}
	//else if (message == WM_RBUTTONDOWN)
	//{
	//	int x = LOWORD(lParam);
	//	int y = HIWORD(lParam);
	//	theMouse->OnRightPressed(x, y);
	//}
	//else if (message == WM_RBUTTONUP)
	//{
	//	int x = LOWORD(lParam);
	//	int y = HIWORD(lParam);
	//	theMouse->OnRightReleased(x, y);
	//}
	//else if (message == WM_MBUTTONUP)
	//{
	//	int x = LOWORD(lParam);
	//	int y = HIWORD(lParam);
	//	theMouse->OnMiddleReleased(x, y);
	//}
	//else if (message == WM_MOUSEWHEEL)
	//{
	//	int x = LOWORD(lParam);
	//	int y = HIWORD(lParam);
	//	if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
	//	{
	//		theMouse->OnWheelUp(x, y);
	//	}
	//	else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
	//	{
	//		theMouse->OnWheelDown(x, y);
	//	}
	//}
	//else if (message == WM_XBUTTONDOWN)
	//{
	//	int x = LOWORD(lParam);
	//	int y = HIWORD(lParam);

	//	if (GET_XBUTTON_WPARAM(wParam) == 2)
	//	{
	//		theMouse->OnForwardPressed(x, y);
	//	}
	//	if (GET_XBUTTON_WPARAM(wParam) == 1)
	//	{
	//		theMouse->OnBackPressed(x, y);
	//	}
	//}
	//else if (message == WM_XBUTTONUP)
	//{
	//	int x = LOWORD(lParam);
	//	int y = HIWORD(lParam);
	//	if (GET_XBUTTON_WPARAM(wParam) == 2)
	//	{
	//		theMouse->OnForwardReleased(x, y);
	//	}
	//	if (GET_XBUTTON_WPARAM(wParam) == 1)
	//	{
	//		theMouse->OnBackPressed(x, y);
	//	}
	//}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

System::System(HINSTANCE hInstance, LPCSTR name, int nCmdShow)
{
	System::fusk = this;

	this->hinstance = hInstance;
	this->applicationName = name;
	this->hwnd = InitWindow(this->hinstance, HEIGHT, WIDTH); 
	this->nCMDShow = nCmdShow;
	this->msg = { 0 };
	this->theGraphicDevice = new GraphicsDevice();

	//backfaceCull
	//this->freezeCheck = false;
	//this->cullingPos = { 0,0,0 };

	this->mouseShow = true;
	//this->mouseSwitch = true;
	//this->moveScreen = true;

	static bool raw_input_initialized = false; 
	if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rawInputDevice;
		rawInputDevice.usUsagePage = 0x01; 
		rawInputDevice.usUsage = 0x02;
		rawInputDevice.dwFlags = 0;
		rawInputDevice.hwndTarget = nullptr;
		if (RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice)) == false)
		{
			OutputDebugStringA("\n\nFailed to register raw input devices.\n\n");
			exit(-1);
		}
		raw_input_initialized = true;
	}
}

System::~System()
{
	delete this->theCamera;
	//delete this->theForwardShader;
	delete this->theMouse;
	delete this->theKeyboard;
	delete this->theGamePad;
	delete this->theTracker;
	delete this->theModelLoader;
	delete this->shaderManager;
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	this->theGraphicDevice->shutDown();
	delete this->theGraphicDevice;

	delete this->skybox;
	delete this->handler;

	delete System::states[0];
	delete System::states[1];
	delete System::commonStates;
	delete System::spriteBatch;
	delete System::fontComicSans;
	delete System::fontArial;
	delete System::physices;
	delete System::debugDraw;
	delete System::soundManager;
}

bool System::initialize()
{
	this->skybox = new Skybox;
	this->skybox->initialize();
	this->handler = new GameObjectHandler;
	//this->handler->initialize();
	this->theCamera = new Camera;
	this->theCamera->SetPosition(0, 6, -15);
	this->theCamera->SetRotation(15, 0, 0);
	//this->camPos = { 0,1,-2.f };
	//this->camRot = { 30.f,0,0 };
	//this->theForwardShader = new ForwardShader;
	this->theKeyboard = new Keyboard;
	this->theMouse = new Mouse;
	this->theGamePad = new GamePad;
	this->theTracker = new GamePad::ButtonStateTracker;
	this->theModelLoader = new ModelLoader;
	
	//this->theForwardShader->initialize();
	//this->obj[0] = new GameObject(shaderManager->getForwardShader());
	//this->obj[1] = new GameObject(shaderManager->getForwardShader());
		


	//D3D10_CULL_BACK; //Test
	//theModelLoader->loadGO(obj[0], "Resources/Models/cube2.lu", "lovelive.tga"); //Library test //load anim_test6
	//this->obj[0]->setScale(3, 2, 1);
	//this->obj[0]->setPosition(1, -0.5, 0);
	//this->obj[1]->setScale(0.5f, 0.5f, 0.5f);
	//theModelLoader->loadGO(obj[1], "Resources/Models/fox_test.lu", "lovelive.tga"); //Library test //load anim_test6
	//this->obj[1]->setPosition(-1, 0.5, 0);
				
	

	//this->handler.addObject(this->obj[1]);
	//this->handler.addObject(this->obj[0]);
	

	System::commonStates = new CommonStates(System::getDevice());
	System::spriteBatch = new SpriteBatch(System::getDeviceContext());
	System::fontComicSans = new SpriteFont(System::getDevice(), L"./Fonts/comic_sans.spritefont");
	System::fontArial = new SpriteFont(System::getDevice(), L"./Fonts/arial.spritefont");
	System::physices = new Physics();
	System::debugDraw = new DEBUG_DRAW();

	System::states.push_back(new MainMenu());
	System::states[MAINMENU]->initailize();
	System::states.push_back(new GunGameState());
	System::states[GUNGAME]->initailize();

	return true;
}

void System::mouseMovement(float deltaTime)
{
	int mouseX = 0;
	int mouseY = 0;
	int sensitivity = 20;

	while (!this->theMouse->EventBufferIsEmpty())
	{
		MouseEvent mEvent = theMouse->ReadEvent();
		if (mEvent.GetType() == MouseEventType::RAW_MOVE)
		{
			mouseX = mEvent.GetPosX();
			mouseY = mEvent.GetPosY();
		}
	}
	//if (moveScreen == false) //
	//{
	//	mouseY = 0;
	//	mouseX = 0;
	//}
	if (abs(camRot.x) > 87.f)
	{
		mouseY = int(abs(mouseY) * camRot.x / -abs(camRot.x));
	}
	this->camRot.x += mouseY * sensitivity* deltaTime;
	this->camRot.y += mouseX * sensitivity* deltaTime;

	if (abs(this->camRot.y) >= 360)
		this->camRot.y = 0;
	if (this->camRot.y > 180)
		this->camRot.y = -180;
	if (this->camRot.y < -180)
		this->camRot.y = 180;
	theCamera->SetRotation(camRot);
}

void System::initImgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); // (void)io;
	ImGui_ImplWin32_Init(this->hwnd);
	ImGui_ImplDX11_Init(this->theGraphicDevice->getDevice(), this->theGraphicDevice->getDeviceContext());
	ImGui::StyleColorsDark();
}

//void System::renderImgui()
//{
//	//ImGui_ImplDX11_NewFrame();
//	//ImGui_ImplWin32_NewFrame();
//	//ImGui::NewFrame();
//	//std::string textUse;
//	//ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//	//
//	//ImGui::Checkbox("Enable automatic-camera ", &guiCam);
//	//if (guiCam == false)
//	//{
//	//	ImGui::SliderFloat("Camera X-Position", &camPos.x, -10.0f, 10.0f);
//	//	ImGui::SliderFloat("Camera Y-Position", &camPos.y, -10.0f, 10.0f);
//	//	ImGui::SliderFloat("Camera Z-Position", &camPos.z, -200.0f, 10.0f);
//	//	theCamera->SetPosition(camPos);
//	//}
//	//ImGui::SliderFloat("Camera X-Rotation", &camRot.x, -90.0f, 90.0f);
//	//ImGui::SliderFloat("Camera Y-Rotation", &camRot.y, -180.0f, 180.0f);
//	//theCamera->SetRotation(camRot);
//	////ImGui::ColorEdit3("bg-color", (float*)&this->color);
//
//	////ImGui::SliderInt("Deferred Render", &this->texToShow, 0, 4);
//	//ImGui::Checkbox("Freeze culling ", &freezeCheck);
//	////textUse = "Mouse pick: " + this->mouseObject + ". ";
//	////ImGui::Text(textUse.c_str());
//	////textUse = "Height from 'Ground': " + std::to_string(this->height) + "m";
//	////ImGui::Text(textUse.c_str());
//
//	//ImGui::Text(("Max player 0: "
//	//	+ std::to_string(obj[1]->getCollisionBox().Max.x) + " "
//	//	+ std::to_string(obj[1]->getCollisionBox().Max.y) + " "
//	//	+ std::to_string(obj[1]->getCollisionBox().Max.z)).c_str());
//
//	//ImGui::Text(("Min player 0: "
//	//	+ std::to_string(obj[1]->getCollisionBox().Min.x) + " "
//	//	+ std::to_string(obj[1]->getCollisionBox().Min.y) + " "
//	//	+ std::to_string(obj[1]->getCollisionBox().Min.z)).c_str());
//
//	//if (collide == true)
//	//{
//	//	ImGui::Text("collision yes");
//	//}
//	//else if(collide == false)
//	//{
//	//	ImGui::Text("collision no");
//	//}
//	////ImGui::SliderInt("Player: ", &this->currentInput, 0, 5);
//	//ImGui::TextColored(ImVec4(1, 1, 0, 1), "Controllers");
//	//ImGui::BeginChild("Scrolling");
//	//for (int n = 0; n < 4; n++)
//	//{
//	//	if (theGamePad->GetState(n).connected==true)
//	//		ImGui::Text("%02d: Connected",n);
//	//	else 
//	//		ImGui::Text("%02d: Disconnected",n);
//	//}
//	//ImGui::EndChild();
//	//ImGui::CaptureKeyboardFromApp(true);
//
//	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//	//ImGui::End();
//
//}

void System::update(float deltaTime)
{
	if (theKeyboard->KeyIsPressed('R'))
	{
		theCamera->rotate(0, 1, 0);
	}
	//this->theCamera->calcCamera(players[0]->getPosition(), players[1]->getPosition(), players[2]->getPosition(), players[3]->getPosition());

	if (theKeyboard->KeyIsPressed('W'))
	{
		theCamera->move(0, 0, 10 * deltaTime);
	}
	else if (theKeyboard->KeyIsPressed('S'))
	{
		theCamera->move(0, 0, -10 * deltaTime);
	}
	
	if (theKeyboard->KeyIsPressed('D'))
	{
		theCamera->move(10 * deltaTime, 0, 0);
	}
	else if (theKeyboard->KeyIsPressed('A'))
	{
		//this->obj2->move(-1 * deltaTime, 0, 0);
		theCamera->move(-10 * deltaTime, 0, 0);
	}
	if (theKeyboard->KeyIsPressed('X'))
	{
		theCamera->move(0, -1 * deltaTime, 0);
	}
	if (theKeyboard->KeyIsPressed('Z'))
	{
		theCamera->move(0, 1 * deltaTime, 0);
	}

	if (theKeyboard->KeyIsPressed('V'))
	{
		//this->change(this->moveScreen);
		this->mouseMovement(deltaTime);
		//this->mouseShow = false;
	}
	else
	//	this->mouseShow = true;
	//ShowCursor(mouseShow);

	//if (theKeyboard->KeyIsPressed('N'))
	//{
	//	this->change(this->mouseShow);
	//	ShowCursor(this->mouseShow);
	//}

	if (theMouse->IsLeftDown())
	{
		std::string str = std::to_string(this->theMouse->GetPosX()) + "\n";
		OutputDebugStringA(str.c_str());
	}
	if (theKeyboard->KeyIsPressed('N')) {
		/*if (TESTMAP != currentState) {
			this->currentState = TESTMAP;
		}*/
	}

	if (theKeyboard->KeyIsPressed('M'))
	{
		this->soundManager->playLooped(L"Battle_Theme_One_1.0.wav");
	}
	if (theKeyboard->KeyIsPressed('N'))
	{
		this->soundManager->stopLooped();
	}

	//theCamera->SetRotation(theMouse->GetPos().y, 0, 0);
	
	physices->Update(deltaTime);

	System::states[System::currentState]->update(deltaTime);
	System::soundManager->update();
}

void System::render()
{
	
	float color[] =
	{
		0.0f,0.0f,0.0f,1.0f
	};

	//renderImgui();

	theGraphicDevice->beginScene(color);//clear the back and depth buffer set depthStencilState
	
	//render imgui in states render
	this->theCamera->Render();
	
	//if (freezeCheck == false)
	//{
	//	this->cullingPos = this->camPos;
	//	shaderManager->getForwardShader()->setCamPosToMatricesPerFrame(this->camPos); //this->camPos
	//}
	//else shaderManager->getForwardShader()->setCamPosToMatricesPerFrame(this->cullingPos);


	DirectX::XMMATRIX camWorld = DirectX::XMMatrixTranslation(this->theCamera->GetPosition().x, this->theCamera->GetPosition().y, this->theCamera->GetPosition().z);

	shaderManager->getForwardShader()->setCamPosToMatricesPerFrame(this->theCamera->GetPosition());
	shaderManager->getForwardShader()->setViewProj(this->theCamera->GetViewMatrix(), this->theGraphicDevice->getProj(), DirectX::XMFLOAT4(this->theCamera->GetPosition().x, this->theCamera->GetPosition().y, this->theCamera->GetPosition().z, 1.0f));

	shaderManager->getDefShader()->setCamPosToMatricesPerFrame(this->theCamera->GetPosition());
	shaderManager->getDefShader()->setViewProj(this->theCamera->GetViewMatrix(), this->theGraphicDevice->getProj(), DirectX::XMFLOAT4(this->theCamera->GetPosition().x, this->theCamera->GetPosition().y, this->theCamera->GetPosition().z, 1.0f));

	shaderManager->getLightShader()->setCamPosToMatricesPerFrame(this->theCamera->GetPosition());
	shaderManager->getLightShader()->setViewProj(this->theCamera->GetViewMatrix(), this->theGraphicDevice->getProj(), DirectX::XMFLOAT4(this->theCamera->GetPosition().x, this->theCamera->GetPosition().y, this->theCamera->GetPosition().z, 1.0f));

	shaderManager->getShadowMapping()->setWorld(DirectX::XMMatrixIdentity());
	shaderManager->getShadowMapping()->setViewProj(this->theCamera->GetViewMatrix(), this->theGraphicDevice->getOrtho(), DirectX::XMFLOAT4(this->theCamera->GetPosition().x, this->theCamera->GetPosition().y, this->theCamera->GetPosition().z, 1.0f));


	this->skybox->setViewProj(this->theCamera->GetViewMatrix(), this->theGraphicDevice->getProj());
	this->skybox->setWorld(camWorld);
	
	if(currentState==MAINMENU)
		this->resetShaders();
	System::states[System::currentState]->render();

	//physices->Update();
	if(currentState==GUNGAME)
		debugDraw->Draw(this->theCamera->GetViewMatrix(), this->theGraphicDevice->getProj());
	System::getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
    ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	theGraphicDevice->presentScene();//EndScene() Present swapchain. Present the backbuffer to the screen

}

void System::resetShaders()
{
	System::getDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->PSSetShader(nullptr, nullptr, 0);
	System::getDeviceContext()->IASetInputLayout(nullptr);
}

void System::run()
{

	if (this->hwnd)
	{
		this->theWindow.height = HEIGHT;
		this->theWindow.width = WIDTH;
		theGraphicDevice->initialize(WIDTH, HEIGHT ,true , hwnd, false, 0.1f, 500.0f,90.0f);

		this->shaderManager = new ShaderManager;
		this->shaderManager->initialize(HEIGHT, WIDTH, 0.1f, 100.0f);
		this->initialize();
		initImgui();

		ShowWindow(this->hwnd, this->nCMDShow);

		//Needs to initialize after ShowWindow, or else it fails!
		this->soundManager = new SoundManager();

		//graphics->initImgui(this->hwnd);
		/*Model** model;*/
		//theModelLoader->loadModel(this->model, "Resources\\Models\\anim_test3.lu"); //Library test //load anim_test4
		//btRigidBody* test;
		//btDynamicsWorld* world;
		//btDispatcher* dispatcher;
		//btBroadphaseInterface* broadphase;
		//btConstraintSolver* solver;
		//btCollisionConfiguration* collisionConfig;
		//this->collisionConfig = new btDefaultCollisionConfiguration();
		
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//Game
				//make keyboard stuff into private function´?
				float delteTime = ImGui::GetIO().DeltaTime;
				update(delteTime);				
				render();

				int xMouse = 0;
				int yMouse = 0;
				
			}
		}
		//shutDown();  //<---Called in D3DMain after run
	}

}

void System::shutDown()
{

}

WPARAM System::getMsgWParam()
{
	return WPARAM();
}

ID3D11Device *& System::getDevice()
{
	return theGraphicDevice->getDevice();
}

ID3D11DeviceContext *& System::getDeviceContext()
{
	return theGraphicDevice->getDeviceContext();
}

WindowClient System::getWindowArea()
{
	return theWindow;
}

SpriteBatch * System::getSpriteBatch()
{
	return System::spriteBatch;
}

CommonStates * System::getCommonStates()
{
	return System::commonStates;
}

SoundManager * System::getSoundManager()
{
	return System::soundManager;
}

SpriteFont * System::getFontComicSans()
{
	return System::fontComicSans;
}

SpriteFont * System::getFontArial()
{
	return System::fontArial;
}

Physics* System::getphysices()
{
	return System::physices;
}

DEBUG_DRAW* System::getDebugDraw()
{
	return System::debugDraw;
}

void System::closeWindow()
{
	SendMessage(System::fusk->hwnd, WM_CLOSE, 0, 0);
}

void System::setState(GameState state)
{
	System::fusk->currentState = state;

	if (state == MAINMENU)
	{
		static_cast<MainMenu*>(System::fusk->states[MAINMENU])->setCurrentMenu(MAIN, true);
	}
}
