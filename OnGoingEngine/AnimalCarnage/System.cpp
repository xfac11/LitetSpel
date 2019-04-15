#include"System.h"
#include "MainMenu.h"
//Keyboard* System::theKeyboard = 0;//for static
GraphicsDevice* System::theGraphicDevice = 0;
Mouse* System::theMouse = 0;
Keyboard* System::theKeyboard = 0;
std::vector<State*> System::states = std::vector<State*>();
GameState System::currentState = GameState::MAINMENU;

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
	else if (message == WM_MBUTTONDOWN)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		theMouse->OnMiddlePressed(x, y);
	}
	else if (message == WM_LBUTTONUP)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		theMouse->OnLeftReleased(x, y);
	}
	else if (message == WM_RBUTTONDOWN)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		theMouse->OnRightPressed(x, y);
	}
	else if (message == WM_RBUTTONUP)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		theMouse->OnRightReleased(x, y);
	}
	else if (message == WM_MBUTTONUP)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		theMouse->OnMiddleReleased(x, y);
	}
	else if (message == WM_MOUSEWHEEL)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			theMouse->OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			theMouse->OnWheelDown(x, y);
		}
	}
	else if (message == WM_XBUTTONDOWN)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (GET_XBUTTON_WPARAM(wParam) == 2)
		{
			theMouse->OnForwardPressed(x, y);
		}
		if (GET_XBUTTON_WPARAM(wParam) == 1)
		{
			theMouse->OnBackPressed(x, y);
		}
	}
	else if (message == WM_XBUTTONUP)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_XBUTTON_WPARAM(wParam) == 2)
		{
			theMouse->OnForwardReleased(x, y);
		}
		if (GET_XBUTTON_WPARAM(wParam) == 1)
		{
			theMouse->OnBackPressed(x, y);
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

System::System(HINSTANCE hInstance, LPCSTR name, int nCmdShow)
{
	this->hinstance = hInstance;
	this->applicationName = name;
	this->hwnd = InitWindow(this->hinstance, HEIGHT, WIDTH); //really?
	this->nCMDShow = nCmdShow;
	this->msg = { 0 };
	this->theGraphicDevice = new GraphicsDevice();
	/*graphics = nullptr;
	graphics = new Graphics;
	theKeyboard = nullptr;
	theKeyboard = new Keyboard;
	theMouse = nullptr;
	theMouse = new Mouse;*/
	//theKeyboard->EnableAutoRepeatChars();
	this->mouseSwitch = true;
	this->flySwitch = true;
	this->moveScreen = true;
	//this->forward = Neutral;
	//this->left_right = Neutral;
	//this->up_down = Neutral;

	static bool raw_input_initialized = false; //can this be local variable?
	if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rawInputDevice;
		rawInputDevice.usUsagePage = 0x01; //Mouse?
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
	delete this->obj;
	delete this->theCamera;
	delete this->theForwardShader;
	delete this->theMouse;
	delete this->theKeyboard;
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	this->theGraphicDevice->shutDown();
	delete this->theGraphicDevice;

	delete System::states[0];
}

bool System::initialize()
{
	this->theCamera = new Camera;
	this->theForwardShader = new ForwardShader;
	this->theKeyboard = new Keyboard;
	this->theMouse = new Mouse;
	this->theForwardShader->initialize();
	this->obj = new GameObject(this->theForwardShader);
	std::vector<Vertex3D> mesh;
	Vertex3D temp[] = {
		DirectX::XMFLOAT4(-0.500000,-0.500000, 0.500000,1.0f),
		DirectX::XMFLOAT2(1,0),
		DirectX::XMFLOAT4(0,0,-1,0),
		DirectX::XMFLOAT4(.500000, -0.500000, 0.500000,1.0f),
		DirectX::XMFLOAT2(1,0),
		DirectX::XMFLOAT4(0,0,-1,0),
		DirectX::XMFLOAT4(-.500000, 0.500000, 0.500000,1.0f),
		DirectX::XMFLOAT2(1,0),
		DirectX::XMFLOAT4(0,0,-1,0),
		DirectX::XMFLOAT4(.500000, 0.500000, 0.500000,1.0f),
		DirectX::XMFLOAT2(1,0),
		DirectX::XMFLOAT4(0,0,-1,0)
	};
	DWORD indices[] = {
		1, 0, 2, 1, 2, 3
	};
	for (int i = 0; i < 4; i++)
	{
		mesh.push_back(temp[i]);
	}
	this->obj->setMesh(mesh, indices, 6);
	this->obj->setScale(0.5, 0.5, 0.5);

	System::states.push_back(new MainMenu());
	System::states[MAINMENU]->initailize();


	return true;
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

	if (theKeyboard->KeyIsPressed('W'))
	{
		theCamera->move(0, 0, -1);
	}
	else if (theKeyboard->KeyIsPressed('S'))
	{
		theCamera->move(0, 0, 1);
	}
	if (theKeyboard->KeyIsPressed('D'))
	{
		this->obj->move(1 * deltaTime, 0, 0);
	}
	else if (theKeyboard->KeyIsPressed('A'))
	{
		this->obj->move(-1 * deltaTime, 0, 0);
	}
	if (theKeyboard->KeyIsPressed('X'))
	{

	}
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
	if (theKeyboard->KeyIsPressed('B')) {
		if (MAINMENU != currentState) {
			this->currentState = MAINMENU;
		}
	}



	theCamera->SetRotation(theMouse->GetPos().y, 0, 0);

	System::states[System::currentState]->update(deltaTime);
}



void System::render()
{
	float color[] =
	{
		1.0f,0.1f,0.5f,1.0f
	};
	ImGui_ImplDX11_NewFrame(); 
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame(); 
	std::string textUse;  
	ImGui::Begin("Hello, world!");
	ImGui::CaptureKeyboardFromApp(true);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	theGraphicDevice->beginScene(color);//clear the back and depth buffer set depthStencilState
	
	//render imgui in states render
	this->theCamera->Render();
	
	this->theForwardShader->setViewProj(this->theCamera->GetViewMatrix(), this->theGraphicDevice->getProj(), DirectX::XMFLOAT4(this->theCamera->GetPosition().x, this->theCamera->GetPosition().y, this->theCamera->GetPosition().z, 1.0f));
	this->theForwardShader->setShaders();//tänker att man kör denna sen renderar allla som använder denna shader sen tar setshader på nästa osv.
	this->obj->draw();
	
	//System::states[System::currentState]->render();
	System::getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
    ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	theGraphicDevice->presentScene();//EndScene() Present swapchain. Present the backbuffer to the screen

}



void System::run()
{

	if (this->hwnd)
	{
		theGraphicDevice->initialize(WIDTH, HEIGHT,true , hwnd, false, 0.1f, 500.0f);
		this->initialize();
		initImgui();
		ShowWindow(this->hwnd, this->nCMDShow);
		//graphics->initImgui(this->hwnd);
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
				update(ImGui::GetIO().DeltaTime);
				render();

				int xMouse = 0;
				int yMouse = 0;
				//
				//while (!this->theMouse->EventBufferIsEmpty())
				//{
				//	MouseEvent mEvent = theMouse->ReadEvent();
				//	//std::string posMsg = "Mouse pos: X=" + std::to_string(mEvent.GetPosX()-384) + ", Y="+ std::to_string(mEvent.GetPosY()-384) + "\n";
				//	if (mEvent.GetType() == MouseEventType::RAW_MOVE)
				//	{
				//		//std::string rawMsg = "raw x: " + std::to_string(mEvent.GetPosX()) + ", Y:" + std::to_string(mEvent.GetPosY()) + "\n";
				//		//OutputDebugStringA(rawMsg.c_str());
				//		xMouse = mEvent.GetPosX();
				//		yMouse = mEvent.GetPosY();
				//	}
				//	/*				if (mEvent.GetType() == LPress)
				//					{
				//						OutputDebugStringA("Pressed LeftMouse\n");
				//					}
				//					if (mEvent.GetType() == LRelease)
				//					{
				//						OutputDebugStringA("Released LeftMouse\n");
				//					}
				//					if (mEvent.GetType() == RPress)
				//					{
				//						OutputDebugStringA("Pressed RightMouse\n");
				//					}
				//					if (mEvent.GetType() == RRelease)
				//					{
				//						OutputDebugStringA("Released RightMouse\n");
				//					}
				//					if (mEvent.GetType() == MPress)
				//					{
				//						OutputDebugStringA("Pressed MiddleMouse\n");
				//					}
				//					if (mEvent.GetType() == MRelease)
				//					{
				//						OutputDebugStringA("Released MiddleMouse\n");
				//					}
				//					if (mEvent.GetType() == WheelUp)
				//					{
				//						OutputDebugStringA("WheelUp\n");
				//					}
				//					if (mEvent.GetType() == WheelDown)
				//					{
				//						OutputDebugStringA("WheelDown\n");
				//					}
				//					if (mEvent.GetType() == FPress)
				//					{
				//						OutputDebugStringA("Pressed FrontMouse\n");
				//					}
				//					if (mEvent.GetType() == FRelease)
				//					{
				//						OutputDebugStringA("Released FrontMouse\n");
				//					}
				//					if (mEvent.GetType() == BPress)
				//					{
				//						OutputDebugStringA("Pressed BackMouse\n");
				//					}
				//					if (mEvent.GetType() == BRelease)
				//					{
				//						OutputDebugStringA("Released BackMouse\n");
				//					}*/

				/*
				while (!theKeyboard->CharBufferIsEmpty()) //decide if or while
				{
					unsigned char theChar = theKeyboard->ReadChar();
					std::string theMsg = "Character: ";
					theMsg += theChar;
					theMsg += "\n";
					//OutputDebugStringA(theMsg.c_str());
					//for typing
					//char inputs
				}
				while (!theKeyboard->KeyBufferIsEmpty())
				{
					KeyboardEvent keyEvent = theKeyboard->ReadKey();
					unsigned char theKey = keyEvent.GetKeyCode();
					char theKey = theKey;
					std::string theMsg = "Key ";
					if (keyEvent.IsPress())
					{
						theMsg += "Pressed: ";
						theMsg += theKey;

						if (theKeyboard->KeyIsPressed('W'))
							forward = Positive;
						else if (theKey == 'S')
							forward = Negative;
						if (theKey == 'D')
							left_right = Positive;
						else if (theKey == 'A')
							left_right = Negative;
						if (theKey == 32) //32 == space
							up_down = Positive;
						else if (theKey == 16) //shift
							up_down = Negative;

						if (theKey == 'B')
						{
							this->change(this->flySwitch);
						}
						if (theKey == 'M')
						{
							this->change(this->mouseSwitch);
							ShowCursor(this->mouseSwitch);
						}
						if (theKey == 9) //tab
						{
							moveScreen = false;
						}
					}
					if (keyEvent.IsRelease())
					{
						theMsg += "Released: ";
						theMsg += theKey;


						if (theKey == 'W')
							forward = Neutral;
						else if (theKey == 'S')
							forward = Neutral;
						if (theKey == 'A')
							left_right = Neutral;
						else if (theKey == 'D')
							left_right = Neutral;
						if (theKey == 32) //32 == space
							up_down = Neutral;
						else if (theKey == 16) //shift
							up_down = Neutral;
						if (theKey == 9) //tab
						{
							moveScreen = true;
						}

					}
					theMsg += "\n";
					OutputDebugStringA(theMsg.c_str());
				}

				if (this->mouseSwitch == false)
					SetCursorPos(400, 400);

				if (moveScreen == false)
				{
					xMouse = 0;
					yMouse = 0;
				}
				graphics->move(forward, left_right, this->up_down, this->flySwitch, xMouse, yMouse);
				graphics->Frame(move1, move2);
				*/
				
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
