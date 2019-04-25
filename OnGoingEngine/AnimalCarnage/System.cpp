#include"System.h"
#include "MainMenu.h"
#include "GunGameState.h"

//Keyboard* System::theKeyboard = 0;//for static
GraphicsDevice* System::theGraphicDevice = 0;
Mouse* System::theMouse = 0;
Keyboard* System::theKeyboard = 0;
GamePad* System::theGamePad = 0;
GamePad::ButtonStateTracker* System::theTracker = 0;
RumbleTemp System::theRumble[4] = {};
ModelLoader* System::theModelLoader = 0;
std::vector<State*> System::states = std::vector<State*>();
GameState System::currentState = GameState::MAINMENU;
System* System::fusk = nullptr;
CommonStates* System::commonStates = nullptr;
SpriteBatch* System::spriteBatch = nullptr;
SpriteFont* System::fontComicSans = nullptr;
ShaderManager* System::shaderManager = nullptr;
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
	System::fusk = this;

	this->hinstance = hInstance;
	this->applicationName = name;
	this->hwnd = InitWindow(this->hinstance, HEIGHT, WIDTH); 
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
	//this->playerInputs = new InputHandler;
	this->mouseShow = true;
	this->mouseSwitch = true;
	this->flySwitch = true;
	this->moveScreen = true;
	this->isPressed = false;
	//this->forward = Neutral;
	//this->left_right = Neutral;
	//this->up_down = Neutral;

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

	delete System::states[0];
	delete System::states[1];
	delete System::commonStates;
	delete System::spriteBatch;
	delete System::fontComicSans;
}

bool System::initialize()
{
	this->theCamera = new Camera;
	this->camPos = { 0,0,-2.f };
	//this->theForwardShader = new ForwardShader;
	this->theKeyboard = new Keyboard;
	this->theMouse = new Mouse;
	this->theGamePad = new GamePad;
	this->theTracker = new GamePad::ButtonStateTracker;
	this->theModelLoader = new ModelLoader;
	this->shaderManager = new ShaderManager;
	this->shaderManager->initialize();
	//this->theForwardShader->initialize();
	this->obj[0] = new GameObject(shaderManager->getForwardShader());
	this->obj[1] = new GameObject(shaderManager->getForwardShader());
	
	for (int i = 0; i < 4; i++)
		this->players[i] = new GameObject(shaderManager->getForwardShader());
	




	std::vector<Vertex3D> mesh;
	Vertex3D temp[] = {
		DirectX::XMFLOAT3(-0.500000,-0.500000, 0.500000),	//pos
		DirectX::XMFLOAT2(0,1),								//uv
		DirectX::XMFLOAT3(0,0,-1),							//normal
		DirectX::XMFLOAT3(0,-1,0),							//tangent
		DirectX::XMFLOAT3(-1,0,0),							//binormal
		DirectX::XMFLOAT3(.500000, -0.500000, 0.500000),	//pos
		DirectX::XMFLOAT2(1,1),								//uv
		DirectX::XMFLOAT3(0,0,-1),							//normal
		DirectX::XMFLOAT3(0,-1,0),							//tangent
		DirectX::XMFLOAT3(-1,0,0),							//binormal
		DirectX::XMFLOAT3(-.500000, 0.500000, 0.500000),	//pos
		DirectX::XMFLOAT2(0,0),								//uv
		DirectX::XMFLOAT3(0,0,-1),							//normal
		DirectX::XMFLOAT3(0,-1,0),							//tangent
		DirectX::XMFLOAT3(-1,0,0),							//binormal
		DirectX::XMFLOAT3(.500000, 0.500000, 0.500000),		//pos
		DirectX::XMFLOAT2(1,0),								//uv
		DirectX::XMFLOAT3(0,0,-1),							//normal
		DirectX::XMFLOAT3(0,-1,0),							//tangent
		DirectX::XMFLOAT3(-1,0,0),							//binormal
	};
	DWORD indices[] = {
		1, 0, 2, 1, 2, 3
	};
	for (int i = 0; i < 4; i++)
	{
		mesh.push_back(temp[i]);
	}

	std::vector<Vertex3D> mesh2;
	Vertex3D temp2[] = {
		DirectX::XMFLOAT3(-0.500000,0.500000, 0.500000),
		DirectX::XMFLOAT2(1,0),
		DirectX::XMFLOAT3(0,0,-1),
		DirectX::XMFLOAT3(0,-1,0),							//tangent
		DirectX::XMFLOAT3(-1,0,0),							//binormal
		DirectX::XMFLOAT3(00000, 1.000000, 0.500000),
		DirectX::XMFLOAT2(1,0),
		DirectX::XMFLOAT3(0,0,-1),
		DirectX::XMFLOAT3(0,-1,0),							//tangent
		DirectX::XMFLOAT3(-1,0,0),							//binormal
		DirectX::XMFLOAT3(0.500000, 0.500000, 0.500000),
		DirectX::XMFLOAT2(1,0),
		DirectX::XMFLOAT3(0,0,-1),
		DirectX::XMFLOAT3(0,-1,0),							//tangent
		DirectX::XMFLOAT3(-1,0,0),							//binormal
	};
	DWORD indices2[] = {
		0,1,2
	};
	for (int i = 0; i < 3; i++)
	{
		mesh2.push_back(temp2[i]);
	}

	this->obj[0]->addModel(mesh2, indices2, 3);
	this->obj[0]->setScale(2, 0.5, 0.5);
	//this->obj[1]->addModel(mesh2, indices2, 3);
	//this->obj[1]->setScale(2, 1, 1);
	//D3D10_CULL_BACK; //Test
	theModelLoader->loadGO(obj[1], "Resources/Models/cube1.lu"); //Library test //load anim_test6
	//this->obj[1]->setScale(0.5f, 0.5f, 0.5f);
	
	this->players[0]->addModel(mesh, indices, 6);
	this->players[0]->setScale(0.5f, 0.4f, 0.1f);
	this->players[1]->addModel(mesh, indices, 6);
	this->players[1]->setScale(0.6f, 0.8f, 0.1f);
	this->players[2]->addModel(mesh, indices, 6);
	this->players[2]->setScale(0.6f, 0.8f, 0.1f);
	this->players[3]->addModel(mesh, indices, 6);
	this->players[0]->getModel(0)->setTexture("cat.tga");
	this->players[3]->setScale(0.6f, 0.8f, 0.1f);
	this->handler.addObject(this->obj[1]);
	this->handler.addObject(this->obj[0]);
	for (int i = 0; i < 4; i++)
		this->handler.addObject(this->players[i]);
	

	System::commonStates = new CommonStates(System::getDevice());
	System::spriteBatch = new SpriteBatch(System::getDeviceContext());
	System::fontComicSans = new SpriteFont(System::getDevice(), L"./Fonts/comic_sans.spritefont");

	System::states.push_back(new MainMenu());
	System::states[MAINMENU]->initailize();
	System::states.push_back(new GunGameState());
	System::states[GUNGAME]->initailize();

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

void System::renderImgui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	std::string textUse;
	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	
	ImGui::Checkbox("Set camera positon with: Keyboard or Gui: ", &guiCam);
	if (guiCam == false)
	{
		ImGui::SliderFloat("Camera X-Position", &camPos.x, -10.0f, 10.0f);
		ImGui::SliderFloat("Camera Y-Position", &camPos.y, -10.0f, 10.0f);
		ImGui::SliderFloat("Camera Z-Position", &camPos.z, -200.0f, 10.0f);
		theCamera->SetPosition(camPos);
	}
	ImGui::SliderFloat("Camera X-Rotation", &camRot.x, -90.0f, 90.0f);
	ImGui::SliderFloat("Camera Y-Rotation", &camRot.y, -180.0f, 180.0f);
	theCamera->SetRotation(camRot);
	//ImGui::ColorEdit3("bg-color", (float*)&this->color);

	//ImGui::SliderInt("Deferred Render", &this->texToShow, 0, 4);
	//ImGui::Checkbox("Freeze culling ", &freezeCheck);
	//textUse = "Mouse pick: " + this->mouseObject + ". ";
	//ImGui::Text(textUse.c_str());
	//textUse = "Height from 'Ground': " + std::to_string(this->height) + "m";
	//ImGui::Text(textUse.c_str());


	if (collide == true)
	{
		ImGui::Text("collision yes");
	}
	else if(collide == false)
	{
		ImGui::Text("collision no");
	}
	ImGui::SliderInt("Player: ", &this->currentInput, 0, 5);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Controllers");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 4; n++)
	{
		if (theGamePad->GetState(n).connected==true)
			ImGui::Text("%02d: Connected",n);
		else 
			ImGui::Text("%02d: Disconnected",n);
	}
	ImGui::EndChild();
	ImGui::CaptureKeyboardFromApp(true);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

}

void System::update(float deltaTime)
{
	int mouseX = 0;
	int mouseY = 0;
	int sensitivity = 20;
	//while (!this->theMouse->EventBufferIsEmpty())
	//{
	//	MouseEvent mEvent = theMouse->ReadEvent();
	//	if (mEvent.GetType() == MouseEventType::RAW_MOVE)
	//	{
	//		mouseX = mEvent.GetPosX();
	//		mouseY = mEvent.GetPosY();
	//	}
	//}
	//if (moveScreen == false) //
	//{
	//	mouseY = 0;
	//	mouseX = 0;
	//}
	//if (abs(camRot.x) > 87.f)
	//{
	//	mouseY = int(abs(mouseY) * camRot.x / -abs(camRot.x));
	//}
	//this->camRot.x += mouseY * sensitivity* deltaTime;
	//this->camRot.y += mouseX * sensitivity* deltaTime;

	//if (abs(this->camRot.y) >= 360)
	//	this->camRot.y = 0;
	//if (this->camRot.y > 180)
	//	this->camRot.y = -180;
	//if (this->camRot.y < -180)
	//	this->camRot.y = 180;
	//theCamera->SetRotation(camRot);




	//old input logic 
	//bool grounded = true; //platform check

	//DirectX::XMFLOAT2 objF = { obj->getPosition().x,obj->getPosition().y };
	//DirectX::XMFLOAT2 scaleObj = { 0.5f, 0.3f };

	//DirectX::XMFLOAT2 one = { playerOne->getPosition().x,playerOne->getPosition().y };
	//DirectX::XMFLOAT2 scaleOne = { 0.3f,0.4f };
	//DirectX::XMFLOAT2 two = { playerTwo->getPosition().x,playerTwo->getPosition().y };
	//DirectX::XMFLOAT2 scaleTwo = { 0.6f,0.8f };
	//if (this->playerInputs->collision(one, scaleOne, two, scaleTwo))
	//	this->collide = true;
	//else
	//	this->collide = false;


	if (GUNGAME == currentState)
	{	
		GameObject* temp= nullptr;
		temp = obj[0];
		if (currentInput == 0)
		{
			temp = players[0];
		}
		else if (currentInput == 1)
		{
			temp = players[1];
		}
		else if (currentInput == 2)
		{
			temp = players[2];
		
		}
		else if (currentInput == 3)
		{
			temp = players[3];

		}
		else if (currentInput == 4)
		{
			temp = obj[0];

		}
		else if (currentInput == 5)
		{
			temp = obj[1];

		}
		DirectX::XMFLOAT2 playerPos = {temp->getPosition().x, temp->getPosition().y};
		DirectX::XMFLOAT2 playerScale = { temp->getScale().x,temp->getScale().y };
		DirectX::XMFLOAT2 player2Pos = { players[1]->getPosition().x, players[1]->getPosition().y };
		DirectX::XMFLOAT2 player2Scale = { players[1]->getScale().x,players[1]->getScale().y };
		
		
		DirectX::XMFLOAT2 obj1Pos = { obj[0]->getPosition().x, obj[0]->getPosition().y };
		DirectX::XMFLOAT2 obj1Scale = { obj[0]->getScale().x,obj[0]->getScale().y };
		

		GunGameState* gamePtr=nullptr;
		for (int i = 0; i < 4; i++)
		{
			gamePtr = dynamic_cast<GunGameState*>(states[1]);
			if (gamePtr != nullptr)
			{

				if (players[i]->getPosition().y - 0.5f*players[i]->getScale().y > 0.f)//0 is boundry for curent floor
					gamePtr->setGrounded(i, false);
				else
					gamePtr->setGrounded(i, true);
				
				
				if (i == 0)
					temp->move(gamePtr->getDirection(i));
				else
					players[i]->move(gamePtr->getDirection(i));
			}
		}
		for (int i = 0; i < 2; i++)
		{
			gamePtr = dynamic_cast<GunGameState*>(states[1]);
			if (gamePtr != nullptr)
			{

				//gamePtr->collision(playerPos, playerScale, 0, obj1Pos, obj1Scale, 0);

				if (obj[i]->getPosition().y - 0.5f*obj[i]->getScale().y > 0.f)
					gamePtr->setObjGrounded(i, false);
				else
					gamePtr->setObjGrounded(i, true);
			}
		}
	}



	this->theCamera->calcCamera(players[0]->getPosition(), players[1]->getPosition());

	if (theKeyboard->KeyIsPressed('W'))
	{
		theCamera->move(0, 0, 1 * deltaTime);
	}
	else if (theKeyboard->KeyIsPressed('S'))
	{
		theCamera->move(0, 0, -1 * deltaTime);

		
	}
	else if (theKeyboard->KeyIsPressed('S'))
	{
		theCamera->move(0, 0, 1 * deltaTime);
	}
	if (theKeyboard->KeyIsPressed('D'))
	{
		theCamera->move(1 * deltaTime, 0, 0);
	}
	else if (theKeyboard->KeyIsPressed('A'))
	{
		//this->obj2->move(-1 * deltaTime, 0, 0);
		theCamera->move(-1 * deltaTime, 0, 0);
	}

	if (theKeyboard->KeyIsPressed('V'))
	{
		this->change(this->moveScreen);
	}
	if (theKeyboard->KeyIsPressed('N'))
	{
		this->change(this->mouseShow);
		ShowCursor(this->mouseShow);
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




	//theCamera->SetRotation(theMouse->GetPos().y, 0, 0);


	System::states[System::currentState]->update(deltaTime);
}



void System::render()
{
	
	float color[] =
	{
		1.0f,0.1f,0.5f,1.0f
	};

	renderImgui();

	theGraphicDevice->beginScene(color);//clear the back and depth buffer set depthStencilState
	
	//render imgui in states render
	this->theCamera->Render();
	
	shaderManager->getForwardShader()->setViewProj(this->theCamera->GetViewMatrix(), this->theGraphicDevice->getProj(), DirectX::XMFLOAT4(this->theCamera->GetPosition().x, this->theCamera->GetPosition().y, this->theCamera->GetPosition().z, 1.0f));
	shaderManager->getForwardShader()->setShaders();//tänker att man kör denna sen renderar allla som använder denna shader sen tar setshader på nästa osv.
	
	//this->model[0]->setShader(shaderManager->getForwardShader());
	//this->model[0]->draw();
	this->handler.draw();

	/*this->obj->draw();
	this->obj2->draw();*/
	
	//reset for the sprite
	

	//this->obj->draw();
	//this->playerOne->draw();
	//this->playerTwo->draw();

	

	this->resetShaders();
	System::states[System::currentState]->render();

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

		theGraphicDevice->initialize(WIDTH, HEIGHT ,true , hwnd, false, 0.1f, 500.0f);

		this->initialize();
		initImgui();
		ShowWindow(this->hwnd, this->nCMDShow);
		//graphics->initImgui(this->hwnd);
		/*Model** model;*/
		//theModelLoader->loadModel(this->model, "Resources\\Models\\anim_test3.lu"); //Library test //load anim_test4
		
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

				for (int i = 0; i < 4; i++)
				{
					updateRumble(delteTime, i);
				}
				
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
	//delete playerInputs;
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

void System::updateRumble(float deltaTime, int id)
{
	if (theRumble[id].rumbleTime > theRumble[id].rumbleClock)
	{
		theRumble[id].rumbleClock += deltaTime;
		System::theGamePad->SetVibration(id, theRumble[id].rumble.x, theRumble[id].rumble.y);
	}
	else
	{
		theRumble[id].rumbleClock = 0.f;
		theRumble[id].rumbleTime = 0.f;
		System::theGamePad->SetVibration(id, 0, 0);
	}
}

SpriteBatch * System::getSpriteBatch()
{
	return System::spriteBatch;
}

CommonStates * System::getCommonStates()
{
	return System::commonStates;
}

SpriteFont * System::getFontComicSans()
{
	return System::fontComicSans;
}

void System::closeWindow()
{
	SendMessage(System::fusk->hwnd, WM_CLOSE, 0, 0);
}

void System::setState(GameState state)
{
	
	System::fusk->currentState = state;
}
