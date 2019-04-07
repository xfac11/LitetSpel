#include"System.h"

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

	if(message== WM_DESTROY)
		PostQuitMessage(0);
	else if (message == WM_CHAR)
	{
		if (sys != nullptr)
		{
			unsigned char theChar = static_cast<unsigned char>(wParam);
			std::string msg;
			msg.push_back(theChar);
			if (sys->theKeyboard->IsCharsAutoRepeat())
			{
				sys->theKeyboard->OnChar(theChar);
			}
			else
			{
				const bool wasPressed = lParam & 0x40000000;
				if (!wasPressed)
				{
					sys->theKeyboard->OnChar(theChar);
					
				}
			}
			//OutputDebugStringA(msg.c_str());
			//MessageBox(hWnd, msg.c_str(), "Keyboard Input", MB_OK); //L"", L"", ;
		}
	}
	else if (message == WM_KEYDOWN)
	{
		unsigned char theKey = static_cast<unsigned char>(wParam);
		if (sys->theKeyboard->IsKeysAutoRepeat())
		{
			sys->theKeyboard->OnKeyPressed(theKey);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				sys->theKeyboard->OnKeyPressed(theKey);
			}
		}
	}
	else if (message == WM_KEYUP)
	{
		unsigned char theKey = static_cast<unsigned char>(wParam);
		sys->theKeyboard->OnKeyReleased(theKey);
	}
	else if(message == WM_MOUSEMOVE)
	{ 
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		sys->theMouse->OnMouseMove(x, y);
	}
	else if (message == WM_INPUT)
	{
		UINT dataSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
		if (dataSize > 0)
		{
			//std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize); 
			BYTE* rawdata = new BYTE[dataSize];
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata, &dataSize, sizeof(RAWINPUTHEADER))==dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata);
				if (raw->header.dwType==RIM_TYPEMOUSE)
				{
					sys->theMouse->OnMouseRAW(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
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
		sys->theMouse->OnLeftPressed(x, y);
	}
	else if (message == WM_LBUTTONDOWN)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		sys->theMouse->OnRightPressed(x, y);
	}
	else if(message == WM_MBUTTONDOWN)
	{ 
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		sys->theMouse->OnMiddlePressed(x, y);
	}
	else if(message == WM_LBUTTONUP)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		sys->theMouse->OnLeftReleased(x, y);
	}
	else if (message == WM_RBUTTONDOWN)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		sys->theMouse->OnRightPressed(x, y);
	}
	else if (message == WM_RBUTTONUP)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		sys->theMouse->OnRightReleased(x, y);
	}
	else if (message == WM_MBUTTONUP)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		sys->theMouse->OnMiddleReleased(x, y);
	}
	else if (message == WM_MOUSEWHEEL)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			sys->theMouse->OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			sys->theMouse->OnWheelDown(x, y);
		}
	}
	else if (message == WM_XBUTTONDOWN)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		
		if (GET_XBUTTON_WPARAM(wParam)==2)
		{
			sys->theMouse->OnForwardPressed(x, y);
		}
		if (GET_XBUTTON_WPARAM(wParam) ==1)
		{
			sys->theMouse->OnBackPressed(x, y);
		}
	}
	else if (message == WM_XBUTTONUP)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_XBUTTON_WPARAM(wParam) == 2)
		{
			sys->theMouse->OnForwardReleased(x, y);
		}
		if (GET_XBUTTON_WPARAM(wParam) == 1)
		{
			sys->theMouse->OnBackPressed(x, y);
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void System::change(bool & theSwitch)
{
	if (theSwitch == true)
		theSwitch = false;
	else
		theSwitch = true;
}

System::System(HINSTANCE hInstance, LPCSTR name, int nCmdShow)
{
	this->hinstance = hInstance;
	this->applicationName = name;
	this->hwnd = InitWindow(this->hinstance, HEIGHT, WIDTH); //really?
	this->nCMDShow = nCmdShow;
	this->msg = { 0 };
	graphics = nullptr;
	graphics = new Graphics;
	theKeyboard = nullptr;
	theKeyboard = new Keyboard;
	theMouse = nullptr;
	theMouse = new Mouse;
	//theKeyboard->EnableAutoRepeatChars();
	this->mouseSwitch = true;
	this->mouseShow = true;
	this->flySwitch = true;
	this->moveScreen = true;
	this->forward = Neutral;
	this->left_right = Neutral;
	this->up_down = Neutral;


	move1 = false;
	move2 = false;

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
}

bool System::initialize()
{
	
	this->hwnd = InitWindow(this->hinstance, HEIGHT, WIDTH);
	
	return this->hwnd;
}

void System::run()
{
	if (this->hwnd)
	{
		graphics->Initialize(768, 768, this->hwnd);
		ShowWindow(this->hwnd, this->nCMDShow);
		graphics->initImgui(this->hwnd);
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

				
				int xMouse = 0;
				int yMouse = 0;
				int mousePosX = 0;
				int mousePosY = 0;
				unsigned char charKey;

				while (!this->theMouse->EventBufferIsEmpty())
				{
					MouseEvent mEvent = theMouse->ReadEvent();
					//std::string posMsg = "Mouse pos: X=" + std::to_string(mEvent.GetPosX()-384) + ", Y="+ std::to_string(mEvent.GetPosY()-384) + "\n";
					if (mEvent.GetType() == MouseEventType::RAW_MOVE)
					{
						//std::string rawMsg = "raw x: " + std::to_string(mEvent.GetPosX()) + ", Y:" + std::to_string(mEvent.GetPosY()) + "\n";
						//OutputDebugStringA(rawMsg.c_str());
						xMouse = mEvent.GetPosX();
						yMouse = mEvent.GetPosY();
					}
					if (mEvent.GetType() == MouseEventType::Move)
					{
						mousePosX = mEvent.GetPosX();
						mousePosY = mEvent.GetPosY();
						std::string temp = std::to_string(mousePosX) + " " + std::to_string(mousePosY) + "\n";
						OutputDebugStringA(temp.c_str());
					}
					if (mEvent.GetType() == LPress)
					{
						this->mouseLeft = true;
						OutputDebugStringA("Pressed LeftMouse\n");
					}
					if (mEvent.GetType() == LRelease)
					{
						this->mouseLeft = false;
						OutputDebugStringA("Released LeftMouse\n");
					}
						/*			if (mEvent.GetType() == RPress)
									{
										OutputDebugStringA("Pressed RightMouse\n");
									}
									if (mEvent.GetType() == RRelease)
									{
										OutputDebugStringA("Released RightMouse\n");
									}
									if (mEvent.GetType() == MPress)
									{
										OutputDebugStringA("Pressed MiddleMouse\n");
									}
									if (mEvent.GetType() == MRelease)
									{
										OutputDebugStringA("Released MiddleMouse\n");
									}
									if (mEvent.GetType() == WheelUp)
									{
										OutputDebugStringA("WheelUp\n");
									}
									if (mEvent.GetType() == WheelDown)
									{
										OutputDebugStringA("WheelDown\n");
									}
									if (mEvent.GetType() == FPress)
									{
										OutputDebugStringA("Pressed FrontMouse\n");
									}
									if (mEvent.GetType() == FRelease)
									{
										OutputDebugStringA("Released FrontMouse\n");
									}
									if (mEvent.GetType() == BPress)
									{
										OutputDebugStringA("Pressed BackMouse\n");
									}
									if (mEvent.GetType() == BRelease)
									{
										OutputDebugStringA("Released BackMouse\n");
									}*/

				}
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
					charKey = theKey;
					std::string theMsg = "Key ";
					if (keyEvent.IsPress())
					{
						theMsg += "Pressed: ";
						theMsg += theKey;
						
						if (theKey == 'W')
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
						}
						if (theKey == 'N')
						{
							this->change(this->mouseShow);
							ShowCursor(this->mouseShow);
						}
						if (theKey == 'V') //tab
						{
							moveScreen = false;
						}

						if (theKey == 'J')
						{
							move1 = true;
						}
						if (theKey == 'K')
						{
							move2 = true;
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
						if (theKey == 'A' )
							left_right = Neutral;
						else if (theKey == 'D')
							left_right = Neutral;
						if (theKey == 32) //32 == space
							up_down = Neutral;
						else if (theKey == 16) //shift
							up_down = Neutral;
						if (theKey == 'V') //tab
						{
							moveScreen = true;
						}

						if (theKey == 'J')//
						{
							move1 = false;
						}
						if (theKey == 'K')
						{
							move2 = false;
						}
					}
					theMsg += "\n";
					OutputDebugStringA(theMsg.c_str());
				}

				if (this->mouseSwitch == false)
					SetCursorPos(600, 600);
				
				if (moveScreen == false)
				{
					xMouse = 0;
					yMouse = 0;
				}
				graphics->move(forward,left_right, this->up_down, this->flySwitch, xMouse, yMouse, mousePosX, mousePosY, mouseLeft); //mPosX, mPosY , mouseLeft 
				graphics->Frame(move1, move2);
				
			}
		}
		//shutDown();
	}
}

void System::shutDown()
{
	DestroyWindow(this->hwnd);

	this->graphics->Shutdown();

	delete graphics;
	delete theKeyboard;
	delete theMouse;
}

WPARAM System::getMsgWParam()
{
	return this->msg.wParam;
}