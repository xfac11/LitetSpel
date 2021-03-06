#include"System.h"
#include "MainMenu.h"
#include "GunGameState.h"

GraphicsDevice* System::theGraphicDevice = nullptr;
Mouse* System::theMouse = nullptr;
Keyboard* System::theKeyboard = nullptr;
GamePad* System::theGamePad = nullptr;
GamePad::ButtonStateTracker* System::theTracker = nullptr;
ModelLoader* System::theModelLoader = nullptr;
std::vector<State*> System::states = std::vector<State*>();
GameState System::currentState = GameState::MAINMENU;
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
WindowClient System::theWindow = { 1080/2, 1920/2};
//SimpleMath::Matrix System::matrixForSpritebatch = SimpleMath::Matrix::CreateScale(System::fusk->theWindow.height / 1080.0f) * SimpleMath::Matrix::CreateTranslation(System::theWindow.width * 0.25f, System::theWindow.height * 0.25f, 0);
//WindowClient System::theWindow = { 720, 1280 };
Resolution System::resolution = _1280x720;
SimpleMath::Matrix System::matrixForSpritebatch = SimpleMath::Matrix::CreateTranslation(((System::theWindow.width - 1920.0f * (System::theWindow.height / 1080.0f)) * 0.5f) / (System::theWindow.height / 1080.0f), 0, 0) * SimpleMath::Matrix::CreateScale((System::theWindow.height / 1080.0f));
Camera* System::theCamera = nullptr;
AssetManager* System::assetMananger = nullptr;
ParticleManager* System::particleManager = nullptr;

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
		if(theMouse!=nullptr)
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
		if(theMouse!=nullptr)
			theMouse->OnLeftPressed(x, y);
	}
	else if (message == WM_RBUTTONDOWN)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (theMouse != nullptr)
			theMouse->OnRightPressed(x, y);
	}
	else if (message == WM_SIZE)
	{

		RECT rect;
		GetWindowRect(hWnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		if(ImGui::GetCurrentContext()!=nullptr)
			ImGui::GetIO().DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
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
	this->hwnd = InitWindow(this->hinstance, (float)this->theWindow.height, (float)this->theWindow.width);
	this->nCMDShow = nCmdShow;
	this->msg = { 0 };
	this->theGraphicDevice = new GraphicsDevice();

	//backfaceCull
	//this->freezeCheck = false;
	//this->cullingPos = { 0,0,0 };

	//this->mouseShow = true;
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
	delete System::particleManager;
	delete System::assetMananger;
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
	System::assetMananger = new AssetManager();
	System::particleManager = new ParticleManager();
	System::soundManager = new SoundManager();
	System::getSoundManager()->loadEffect(L"Getting_Hit_Punch.wav", "0");
	System::getSoundManager()->loadEffect(L"Getting_Hit_Punch_2.wav", "1");
	System::getSoundManager()->loadEffect(L"Getting_Hit_Punch_3.wav", "2");
	System::getSoundManager()->loadEffect(L"Getting_Hit_Punch_4.wav", "3");
	System::getSoundManager()->loadEffect(L"Wall_Jump_Crash.wav", "4");
	System::getSoundManager()->loadEffect(L"Swing.wav", "5");
	System::getSoundManager()->loadEffect(L"Death.wav", "Death");
	System::getSoundManager()->loadEffect(L"Double_Jump.wav", "Double_Jump");
	System::getSoundManager()->loadEffect(L"Jump.wav", "Jump");
	System::getSoundManager()->loadEffect(L"Stone_Soft_Impact.wav", "Stone_Soft_Impact");
	System::getSoundManager()->loadEffect(L"Stone_Hard_Impact.wav", "Stone_Hard_Impact");
	System::getSoundManager()->loadEffect(L"Stone_Getting_Hit.wav", "Stone_Getting_Hit");
	System::getSoundManager()->loadEffect(L"Stone_Getting_Destroyed.wav", "Stone_Getting_Destroyed");

	shaderManager->getParticleShader()->setCamera(SimpleMath::Vector3(this->theCamera->GetUp()));
	shaderManager->getLightShader()->setWindow(this->theWindow.width, this->theWindow.height);
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.Height = (float)System::fusk->theWindow.height;
	vp.Width = (float)System::fusk->theWindow.width;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	spriteBatch->SetViewport(vp);
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
		this->camRot.y = 0.f;
	if (this->camRot.y > 180.f)
		this->camRot.y = -180.f;
	if (this->camRot.y < -180.f)
		this->camRot.y = 180.f;
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

void System::update(float deltaTime)
{
	GunGameState* ptr = nullptr;
	if (ptr = dynamic_cast<GunGameState*>(states[System::currentState]))
	{
		vector<DirectX::XMFLOAT3> playerPos;
		for (int i = 0; i< ptr->getNrOfPlayers(); i++)
		{
			playerPos.push_back(ptr->getPlayer(i)->getPosition());
		}
		if (ptr->checkPause() == false)
		{
			//remeber to also set rotation
			this->theCamera->calcCamera(playerPos);
		}
		else
		{
			if (ptr->checkCameraFocus() == true) //it sets exact position, remember to change z value to appropriate location
				theCamera->SetPosition(playerPos[ptr->getCameraFocus()].x, playerPos[ptr->getCameraFocus()].y,-5.f);
			theCamera->move(ptr->changeCamera(deltaTime));
			theCamera->rotate(ptr->rotateCamera(deltaTime));
		}
		if (ptr->checkPause() == false)
		{
			physices->Update(deltaTime);
			if (theCamera->GetRotation().x != 0 || theCamera->GetRotation().y != 0 || theCamera->GetRotation().z != 0)
				theCamera->SetRotation(2, 0, 0);
		}
	}

	//if (theKeyboard->KeyIsPressed('R'))
	//{
	//	theCamera->rotate(0, 1, 0);
	//}
	//if (theKeyboard->KeyIsPressed('W'))
	//{
	//	theCamera->move(0, 0, 10 * deltaTime);
	//}
	//else if (theKeyboard->KeyIsPressed('S'))
	//{
	//	theCamera->move(0, 0, -10 * deltaTime);
	//}
	//
	//if (theKeyboard->KeyIsPressed('D'))
	//{
	//	theCamera->move(10 * deltaTime, 0, 0);
	//}
	//else if (theKeyboard->KeyIsPressed('A'))
	//{
	//	//this->obj2->move(-1 * deltaTime, 0, 0);
	//	theCamera->move(-10 * deltaTime, 0, 0);
	//}
	//if (theKeyboard->KeyIsPressed('X'))
	//{
	//	theCamera->move(0, -1 * deltaTime, 0);
	//}
	//if (theKeyboard->KeyIsPressed('Z'))
	//{
	//	theCamera->move(0, 1 * deltaTime, 0);
	//}

	if (theKeyboard->KeyIsPressed('V'))
	{
		this->mouseMovement(deltaTime);
		//this->mouseShow = false;
	}
	//else
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
		this->soundManager->playLooped(L"Main_Theme_2.1.wav");
	}
	if (theKeyboard->KeyIsPressed('N'))
	{
		this->soundManager->stopLooped();
	}

	if (theKeyboard->KeyIsPressed('L'))
	{
		this->resizeWindow(theWindow.width, theWindow.height + 10);
	}
	if (theKeyboard->KeyIsPressed('O'))
	{
		this->resizeWindow(theWindow.width + 10, theWindow.height);
	}
	if (theKeyboard->KeyIsPressed('P'))
	{
		this->resizeWindow(1920, 540);
	}

	//theCamera->SetRotation(theMouse->GetPos().y, 0, 0);
	
	System::states[System::currentState]->update(deltaTime);

	if(System::states[System::currentState])
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
	/*RECT w;
	LPRECT s;
	GetWindowRect(this->hwnd, &w);
	GetClientRect(this->hwnd, s);*/
	
	this->skybox->setViewProj(this->theCamera->GetViewMatrix(), this->theGraphicDevice->getProj());
	this->skybox->setWorld(camWorld);
	
	if(currentState==MAINMENU)
		this->resetShaders();
	System::states[System::currentState]->render();

	//physices->Update();
	/*if (currentState == GUNGAME)
		debugDraw->Draw(this->theCamera->GetViewMatrix(), this->theGraphicDevice->getProj());*/

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
		theGraphicDevice->initialize(this->theWindow.width, this->theWindow.height, true, hwnd, false, 500.0f, 0.1f,60.0f);

		ShowWindow(this->hwnd, this->nCMDShow);

		this->shaderManager = new ShaderManager;
		this->shaderManager->initialize(this->theWindow.height, this->theWindow.width, 0.1f, 500.0f);
		this->initialize();
		initImgui();
		
		//Store counts per second
		__int64 countsPerSec = 0;
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
		float secPerCount = 1.0f / countsPerSec; //store seconds per count

		//Initial previous time	
		__int64 prevTime = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&prevTime);


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
				//make keyboard stuff into private function�?
				//float deltaTime = ImGui::GetIO().DeltaTime;

				//Capture current count
				__int64 curTime = 0;
				QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
				//Calculate deltaTime
				deltaTime = (curTime - prevTime) * secPerCount;
				//deltaTime /= 2;

				update(deltaTime);				
				render();

				int xMouse = 0;
				int yMouse = 0;
				

				//set previous to current after frame ends
				prevTime = curTime;
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

ParticleManager * System::getParticleManager()
{
	return System::particleManager;
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

	if (state != GUNGAME)
	{
		particleManager->resetAndClear();
	}
}

State * System::getCurrentState()
{
	return System::states[System::currentState];
}

void System::resizeWindow(int width, int height)
{
	shaderManager->getDefShader()->gBuffer.resize(height, width);
	theGraphicDevice->resize(width, height);
	shaderManager->getLightShader()->setWindow(width, height);
	shaderManager->getHorBlur()->resize(height, width);
	shaderManager->getVerBlur()->resize(height, width);

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.Height = static_cast<float>(height);
	vp.Width = static_cast<float>(width);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	spriteBatch->SetViewport(vp);


	System::matrixForSpritebatch = SimpleMath::Matrix::CreateTranslation(((width - 1920.0f * (height / 1080.0f)) * 0.5f) / (height / 1080.0f) , 0, 0) * SimpleMath::Matrix::CreateScale((height / 1080.0f));

	RECT rect;
	GetWindowRect(System::fusk->hwnd, &rect);
	MoveWindow(System::fusk->hwnd, rect.left, rect.top, width, height, true);

	ImGui::GetIO().DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));

	System::fusk->theWindow.width = width;
	System::fusk->theWindow.height = height;
}

const SimpleMath::Matrix & System::getSpritebatchMatrix()
{
	return System::matrixForSpritebatch;
}

float System::getDeltaTime()
{
	return this->deltaTime;
}
